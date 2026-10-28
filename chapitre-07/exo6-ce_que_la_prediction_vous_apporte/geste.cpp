// =============================================================================
//  Exercice : Ce que la prédiction vous apporte (Chapitre 07)
// =============================================================================
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>

#include "NKMath/NkQuat.h"
#include "NKMath/NkAngle.h"

using namespace nkentseu;
using namespace nkentseu::math;

namespace {
    const double kPi = 3.14159265358979323846;
    const double kHz = 72.0;
    const float32 dt = static_cast<float32>(1.0 / kHz);

    double LacetProfil(double t, double amplitude, double duree) {
        if (t <= 0.0) return 0.0;
        if (t >= duree) return amplitude;
        return amplitude * 0.5 * (1.0 - cos(kPi * t / duree));
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

    struct ResultatGeste {
        std::string nom;
        double moyAvec;
        double moySans;
        double rapport;
    };

    ResultatGeste SimulerGeste(const std::string &nom, double amplitudeDeg, double dureeSec) {
        std::vector<NkQuatf> posesVraies;
        int nbSamples = static_cast<int>(std::ceil(dureeSec * kHz)) + 1;

        for (int i = 0; i < nbSamples; ++i) {
            double t = i * (1.0 / kHz);
            if (t > dureeSec) t = dureeSec;
            double lacetDeg = LacetProfil(t, amplitudeDeg, dureeSec);
            double lacetRad = lacetDeg * (kPi / 180.0);

            // Utilisation correcte de FromRad avec conversion degrés -> radians
            NkQuatf q = NkQuatf::RotateY(NkAngle::FromRad(static_cast<float32>(lacetRad)));
            posesVraies.push_back(q);
        }

        double sommeErrAvec = 0.0;
        double sommeErrSans = 0.0;
        int count = 0;

        for (size_t i = 1; i < posesVraies.size() - 1; ++i) {
            NkQuatf posePrecedente = posesVraies[i - 1];
            NkQuatf poseCourante = posesVraies[i];
            NkQuatf poseVraieFuture = posesVraies[i + 1];

            NkVec3f vitesse = VitesseAngulaire(posePrecedente, poseCourante, dt);
            NkQuatf posePredite = Extrapoler(poseCourante, vitesse, dt);

            float32 errAvec = (poseVraieFuture * posePredite.Conjugate()).Normalized().Angle().Deg();
            float32 errSans = (poseVraieFuture * poseCourante.Conjugate()).Normalized().Angle().Deg();

            sommeErrAvec += errAvec;
            sommeErrSans += errSans;
            count++;
        }

        double moyAvec = (count > 0) ? (sommeErrAvec / count) : 0.0;
        double moySans = (count > 0) ? (sommeErrSans / count) : 0.0;
        double rapport = (moyAvec > 1e-6) ? (moySans / moyAvec) : 0.0;

        return { nom, moyAvec, moySans, rapport };
    }
} // namespace

int main() {
    ResultatGeste lent = SimulerGeste("lent (45 deg en 1.0 s)", 45.0, 1.0);
    ResultatGeste rapide = SimulerGeste("rapide (90 deg en 400 ms)", 90.0, 0.400);
    ResultatGeste demiTour = SimulerGeste("demi-tour (180 deg en 300 ms)", 180.0, 0.300);

    std::printf("%s : avec %f deg, sans %f deg, rapport %f\n", rapide.nom.c_str(), rapide.moyAvec, rapide.moySans, rapide.rapport);
    std::printf("%s : avec %f deg, sans %f deg, rapport %f\n", lent.nom.c_str(), lent.moyAvec, lent.moySans, lent.rapport);
    std::printf("%s : avec %f deg, sans %f deg, rapport %f\n", demiTour.nom.c_str(), demiTour.moyAvec, demiTour.moySans, demiTour.rapport);

    std::ofstream out("gestes.txt");
    if (out.is_open()) {
        out << "rapide (90 deg en 400 ms) : avec " << rapide.moyAvec << " deg, sans " << rapide.moySans << " deg, rapport " << rapide.rapport << "\n";
        out << "lent (45 deg en 1.0 s) : avec " << lent.moyAvec << " deg, sans " << lent.moySans << " deg, rapport " << lent.rapport << "\n";
        out << "demi-tour (180 deg en 300 ms) : avec " << demiTour.moyAvec << " deg, sans " << demiTour.moySans << " deg, rapport " << demiTour.rapport << "\n";
        out.close();
    }

    return 0;
}