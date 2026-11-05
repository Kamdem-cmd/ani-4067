// =============================================================================
//  Exercice : La latence injectée et la borne (Chapitre 07)
// =============================================================================
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "NKMath/NkQuat.h"
#include "NKMath/NkAngle.h"

using namespace nkentseu;
using namespace nkentseu::math;

namespace {
    const double kPi = 3.14159265358979323846;
    const double kHz = 72.0;
    const float32 dtFrame = static_cast<float32>(1.0 / kHz);
    const uint32 kMaxSamples = 64;

    double LacetProfil(double t) {
        // Mouvement démarré à t = 1.0 s pendant 400 ms (jusqu'à 1.4 s)
        const double tDebut = 1.0;
        const double duree = 0.400;
        const double amplitude = 90.0;

        if (t <= tDebut) return 0.0;
        if (t >= tDebut + duree) return amplitude;
        double tRel = t - tDebut;
        return amplitude * 0.5 * (1.0 - std::cos(kPi * tRel / duree));
    }

    NkVec3f VitesseAngulaire(const NkQuatf &from, const NkQuatf &to, float32 dtSeconds) {
        if (dtSeconds <= 0.f) {
            return NkVec3f(0.f, 0.f, 0.f);
        }
        NkQuatf delta = (to * from.Conjugate()).Normalized();
        if (delta.w < 0.f) {
            delta = NkQuatf(-delta.x, -delta.y, -delta.z, -delta.w);
        }
        const float32 angleRad = delta.Angle().Rad();
        if (angleRad <= 1e-6f) {
            return NkVec3f(0.f, 0.f, 0.f);
        }
        return delta.Axis() * (angleRad / dtSeconds);
    }

    NkQuatf Extrapoler(const NkQuatf &orientation, const NkVec3f &vitesse, float32 dtSeconds) {
        const float32 speedRad = vitesse.Len();
        if (speedRad > 1e-6f) {
            const NkVec3f axis = vitesse * (1.f / speedRad);
            const NkQuatf delta(NkAngle::FromRad(speedRad * dtSeconds), axis);
            return (delta * orientation).Normalized();
        }
        return orientation;
    }

    struct SamplePose {
        double time; // en secondes
        NkQuatf pose;
        NkVec3f velocity;
    };

    struct SimulatorBuffer {
        SamplePose samples[kMaxSamples];
        uint32 head = 0;
        uint32 count = 0;

        void Push(double t, const NkQuatf &p, const NkVec3f &v) {
            if (count == 0) {
                head = 0;
            } else {
                head = (head + 1) % kMaxSamples;
            }
            samples[head] = { t, p, v };
            if (count < kMaxSamples) {
                count++;
            }
        }

        SamplePose SampleAt(double now, double latencySec) const {
            if (latencySec <= 0.0 || count <= 1) {
                return samples[head];
            }
            double target = now - latencySec;
            uint32 index = head;
            for (uint32 i = 0; i < count; ++i) {
                if (samples[index].time <= target) {
                    return samples[index];
                }
                index = (index + kMaxSamples - 1) % kMaxSamples;
            }
            return samples[(head + kMaxSamples - (count - 1)) % kMaxSamples];
        }
    };

    double QuatToYawDeg(const NkQuatf &q) {
        // lacet = 2 * atan2(q.y, q.w) en degrés
        double yawRad = 2.0 * std::atan2(static_cast<double>(q.y), static_cast<double>(q.w));
        return yawRad * (180.0 / kPi);
    }

    struct ResultatLatence {
        int latenceMs;
        double moyAvec;
        double maxAvec;
        double moySans;
        double maxSans;
    };

    ResultatLatence SimulerLatence(double latencyMs) {
        SimulatorBuffer buffer;
        double latencySec = latencyMs / 1000.0;

        // Pré-remplissage du temps de t = 0.0s à t = 3.0s à 72 Hz
        int totalSteps = static_cast<int>(std::round(3.0 * kHz)) + 1;
        std::vector<NkQuatf> vraiesPoses(totalSteps);
        std::vector<double> temps(totalSteps);

        for (int i = 0; i < totalSteps; ++i) {
            temps[i] = i * (1.0 / kHz);
            double lacet = LacetProfil(temps[i]);
            double lacetRad = lacet * (kPi / 180.0);
            vraiesPoses[i] = NkQuatf::RotateY(NkAngle::FromRad(static_cast<float32>(lacetRad)));
        }

        double sumErrAvec = 0.0;
        double maxErrAvec = 0.0;
        double sumErrSans = 0.0;
        double maxErrSans = 0.0;
        int evalCount = 0;

        NkQuatf prevPose = vraiesPoses[0];

        for (int i = 0; i < totalSteps; ++i) {
            double now = temps[i];
            NkQuatf curPose = vraiesPoses[i];

            NkVec3f vel(0.f, 0.f, 0.f);
            if (i > 0) {
                vel = VitesseAngulaire(prevPose, curPose, dtFrame);
            }
            buffer.Push(now, curPose, vel);
            prevPose = curPose;

            // Évaluation de t = 1.0 s à t = 1.9 s
            if (now >= 1.0 && now <= 1.9 && (i + 1) < totalSteps) {
                double targetDisplayTime = now + (1.0 / kHz);
                NkQuatf trueDisplayPose = vraiesPoses[i + 1];

                SamplePose delayed = buffer.SampleAt(now, latencySec);

                // Prédiction avec la borne standard à 100 ms (0.1 s)
                float32 dtPred = static_cast<float32>(targetDisplayTime - delayed.time);
                dtPred = std::clamp(dtPred, -0.1f, 0.1f);

                NkQuatf predPose = Extrapoler(delayed.pose, delayed.velocity, dtPred);

                float32 errAvec = (trueDisplayPose * predPose.Conjugate()).Normalized().Angle().Deg();
                float32 errSans = (trueDisplayPose * delayed.pose.Conjugate()).Normalized().Angle().Deg();

                sumErrAvec += errAvec;
                if (errAvec > maxErrAvec) maxErrAvec = errAvec;

                sumErrSans += errSans;
                if (errSans > maxErrSans) maxErrSans = errSans;

                evalCount++;
            }
        }

        double moyAvec = evalCount > 0 ? sumErrAvec / evalCount : 0.0;
        double moySans = evalCount > 0 ? sumErrSans / evalCount : 0.0;

        return { static_cast<int>(latencyMs), moyAvec, maxErrAvec, moySans, maxErrSans };
    }

    struct InfoBorne {
        double timeSample;
        double horizonMs;
        double lacetPreditDeg;
        double lacetVraiDeg;
    };

    InfoBorne TesterBorne(double latencySec, bool avecBorne) {
        SimulatorBuffer buffer;
        int totalSteps = static_cast<int>(std::round(3.0 * kHz)) + 1;
        std::vector<NkQuatf> vraiesPoses(totalSteps);
        std::vector<double> temps(totalSteps);

        for (int i = 0; i < totalSteps; ++i) {
            temps[i] = i * (1.0 / kHz);
            double lacet = LacetProfil(temps[i]);
            double lacetRad = lacet * (kPi / 180.0);
            vraiesPoses[i] = NkQuatf::RotateY(NkAngle::FromRad(static_cast<float32>(lacetRad)));
        }

        NkQuatf prevPose = vraiesPoses[0];
        InfoBorne res{};

        for (int i = 0; i < totalSteps; ++i) {
            double now = temps[i];
            NkQuatf curPose = vraiesPoses[i];
            NkVec3f vel = (i > 0) ? VitesseAngulaire(prevPose, curPose, dtFrame) : NkVec3f(0.f, 0.f, 0.f);

            buffer.Push(now, curPose, vel);
            prevPose = curPose;

            // Image au milieu du geste : t = 1.2 s
            if (std::abs(now - 1.2) < (0.5 * dtFrame)) {
                double targetDisplayTime = now + (1.0 / kHz);
                NkQuatf trueDisplayPose = vraiesPoses[i + 1];

                SamplePose delayed = buffer.SampleAt(now, latencySec);

                float32 dtPred = static_cast<float32>(targetDisplayTime - delayed.time);
                if (avecBorne) {
                    dtPred = std::clamp(dtPred, -0.1f, 0.1f);
                }

                NkQuatf predPose = Extrapoler(delayed.pose, delayed.velocity, dtPred);

                res.timeSample = delayed.time;
                res.horizonMs = dtPred * 1000.0;
                res.lacetPreditDeg = QuatToYawDeg(predPose);
                res.lacetVraiDeg = QuatToYawDeg(trueDisplayPose);
            }
        }
        return res;
    }
} // namespace

int main() {
    int latencesMs[] = { 0, 10, 20, 40, 80 };
    std::vector<ResultatLatence> resultats;

    for (int l : latencesMs) {
        resultats.push_back(SimulerLatence(static_cast<double>(l)));
    }

    // Test de la borne avec latence = 1.0s à t = 1.2s
    InfoBorne sansBorne = TesterBorne(1.0, false);
    InfoBorne avecBorne = TesterBorne(1.0, true);

    // Écriture du fichier latences.txt
    std::ofstream out("latences.txt");
    if (out.is_open()) {
        for (const auto &r : resultats) {
            out << "latence " << r.latenceMs << " ms : avec " << r.moyAvec << " deg (max " << r.maxAvec
                << "), sans " << r.moySans << " deg (max " << r.maxSans << ")\n";
            std::printf("latence %d ms : avec %.6f deg (max %.6f), sans %.6f deg (max %.6f)\n",
                        r.latenceMs, r.moyAvec, r.maxAvec, r.moySans, r.maxSans);
        }

        out << "sans borne : echantillon a " << sansBorne.timeSample << " s, horizon " << sansBorne.horizonMs
            << " ms, predit " << sansBorne.lacetPreditDeg << " deg, vrai " << sansBorne.lacetVraiDeg << " deg\n";

        out << "avec borne : echantillon a " << avecBorne.timeSample << " s, horizon " << avecBorne.horizonMs
            << " ms, predit " << avecBorne.lacetPreditDeg << " deg, vrai " << avecBorne.lacetVraiDeg << " deg\n";

        std::printf("sans borne : echantillon a %.3f s, horizon %.1f ms, predit %.2f deg, vrai %.2f deg\n",
                    sansBorne.timeSample, sansBorne.horizonMs, sansBorne.lacetPreditDeg, sansBorne.lacetVraiDeg);
        std::printf("avec borne : echantillon a %.3f s, horizon %.1f ms, predit %.2f deg, vrai %.2f deg\n",
                    avecBorne.timeSample, avecBorne.horizonMs, avecBorne.lacetPreditDeg, avecBorne.lacetVraiDeg);

        out.close();
    }

    return 0;
}