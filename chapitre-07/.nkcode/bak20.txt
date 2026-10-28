// =============================================================================
//  Exercice : Prédire et comparer (Chapitre 07)
// =============================================================================
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkMouseEvent.h"

#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRenderer/NkRenderer.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKRenderer/Core/NkRenderGraph.h"
#include "NKRenderer/Core/NkTextureLibrary.h"
#include "NKRenderer/Materials/NkMaterialCollection.h"
#include "NKRenderer/Mesh/NkMeshSystem.h"
#include "NKRenderer/Tools/Offscreen/NkOffscreenTarget.h"
#include "NKRenderer/Tools/Render2D/NkRender2D.h"
#include "NKRenderer/Tools/Render3D/NkRender3D.h"

#include "NKTime/NkChrono.h"
#include "NKMath/NkQuat.h"
#include "NKMath/NkAngle.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData &d) { d.appName = "PredireEtComparer"; }
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

namespace {
    const float32 kPiece = 4.00f;
    const float32 kPlafond = 2.50f;
    const float32 kMur = 0.10f;
    const float32 kYeux = 1.70f;
    const float32 kDemi = kPiece * 0.5f;

    void Pave(NkRender3D *r3d, NkMeshSystem *meshes, const NkVec3f &centre,
              const NkVec3f &taille, const NkVec3f &teinte, float32 rugosite = 1.f) {
        NkDrawCall3D dc;
        dc.mesh = meshes->GetCube();
        dc.transform = NkMat4f::Translate(centre) * NkMat4f::Scale(taille);
        dc.aabb = { { centre.x - taille.x * 0.5f, centre.y - taille.y * 0.5f,
                      centre.z - taille.z * 0.5f },
                    { centre.x + taille.x * 0.5f, centre.y + taille.y * 0.5f,
                      centre.z + taille.z * 0.5f } };
        dc.tint = teinte;
        dc.metallic = 0.f;
        dc.roughness = rugosite;
        r3d->Submit(dc);
    }

    void SoumettreLaSalle(NkRender3D *r3d, NkMeshSystem *meshes) {
        const NkVec3f grisMur = { 0.72f, 0.70f, 0.66f };
        const NkVec3f grisSol = { 0.38f, 0.36f, 0.34f };
        const NkVec3f bois = { 0.55f, 0.38f, 0.22f };
        const NkVec3f vitre = { 0.75f, 0.85f, 0.92f };

        {
            NkDrawCall3D dc;
            dc.mesh = meshes->GetPlane();
            dc.transform = NkMat4f::Scale({ kPiece, 1.f, kPiece });
            dc.aabb = { { -kDemi, 0.f, -kDemi }, { kDemi, 0.f, kDemi } };
            dc.castShadow = false;
            dc.tint = grisSol;
            dc.metallic = 0.f;
            dc.roughness = 1.f;
            r3d->Submit(dc);
        }

        const float32 yMur = kPlafond * 0.5f;
        const float32 bord = kDemi + kMur * 0.5f;

        Pave(r3d, meshes, { 0.f, yMur, -bord }, { kPiece, kPlafond, kMur }, grisMur);
        Pave(r3d, meshes, { 0.f, yMur, bord }, { kPiece, kPlafond, kMur }, grisMur);
        Pave(r3d, meshes, { -bord, yMur, 0.f }, { kMur, kPlafond, kPiece }, grisMur);
        Pave(r3d, meshes, { bord, yMur, 0.f }, { kMur, kPlafond, kPiece }, grisMur);
        Pave(r3d, meshes, { 0.f, kPlafond + kMur * 0.5f, 0.f }, { kPiece, kMur, kPiece }, grisMur);
        Pave(r3d, meshes, { -0.70f, 2.00f * 0.5f, -kDemi + 0.02f }, { 0.90f, 2.00f, 0.04f }, bois, 0.6f);
        Pave(r3d, meshes, { -kDemi + 0.02f, 1.40f, -0.90f }, { 0.04f, 1.00f, 1.20f }, vitre, 0.15f);
        Pave(r3d, meshes, { -kDemi + 0.02f, 1.40f, 0.90f }, { 0.04f, 1.00f, 1.20f }, vitre, 0.15f);

        const float32 tx = 0.60f, tz = 0.70f;
        const float32 ep = 0.05f;
        Pave(r3d, meshes, { tx, 0.75f - ep * 0.5f, tz }, { 1.20f, ep, 0.80f }, bois, 0.5f);

        for (int i = 0; i < 4; ++i) {
            const float32 dx = ((i & 1) ? 1.f : -1.f) * (1.20f * 0.5f - 0.06f);
            const float32 dz = ((i & 2) ? 1.f : -1.f) * (0.80f * 0.5f - 0.06f);
            Pave(r3d, meshes, { tx + dx, (0.75f - ep) * 0.5f, tz + dz }, { 0.06f, 0.75f - ep, 0.06f }, bois, 0.6f);
        }
        Pave(r3d, meshes, { -1.10f, 0.50f, 1.20f }, { 1.f, 1.f, 1.f }, { 0.85f, 0.80f, 0.30f }, 0.8f);
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

    double EnvDouble(const char *nom, double defaut) {
        const char *t = getenv(nom);
        if (t == nullptr || *t == '\0') return defaut;
        return std::atof(t);
    }
} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    // Récupération de la fréquence de l'écran via MASALLE_HZ (défaut : 60 Hz si non défini)
    const double hz = EnvDouble("MASALLE_HZ", 60.0);
    const float32 dtFrame = (hz > 0.0) ? static_cast<float32>(1.0 / hz) : (1.0f / 60.0f);

    NkWindowConfig cfg;
    cfg.title = "Chapitre 07 - Predire et Comparer";
    cfg.width = 1280;
    cfg.height = 720;
    cfg.centered = true;
    cfg.vsync = true;

    NkWindow fenetre(cfg);
    if (!fenetre.IsValid()) {
        std::printf("[Predire] La fenetre n'a pas pu s'ouvrir\n");
        return 1;
    }

    NkDeviceInitInfo devInit{};
    devInit.surface = fenetre.GetSurfaceDesc();
    devInit.width = fenetre.GetSize().x;
    devInit.height = fenetre.GetSize().y;

    NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);
    if (device == nullptr) return 1;

    NkRendererConfig cfgEcran = NkRendererConfig::For2D(devInit.api, devInit.width, devInit.height);
    NkRenderer *rEcran = NkRenderer::Create(device, cfgEcran);
    if (rEcran == nullptr || !rEcran->Initialize()) {
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    NkOffscreenDesc od;
    od.width = devInit.width;
    od.height = devInit.height;
    od.hasDepth = true;
    od.colorFmt = NkGPUFormat::NK_RGBA8_UNORM;
    od.readable = true;
    od.readback = true;
    od.name = "Predire_Cible";

    NkOffscreenTarget *cible = rEcran->CreateOffscreen(od);
    if (cible == nullptr) {
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    NkRendererConfig cfgSalle = NkRendererConfig::ForGame(devInit.api, devInit.width, devInit.height);
    cfgSalle.Disable(NK_SS_RENDER2D);
    cfgSalle.Disable(NK_SS_TEXT);
    cfgSalle.Disable(NK_SS_OVERLAY);
    cfgSalle.Enable(NK_SS_OFFSCREEN);

    NkRenderer *renderer = NkRenderer::Create(device, cfgSalle);
    if (renderer == nullptr || !renderer->Initialize()) {
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }
    renderer->SetFinalColorTarget(rEcran->GetTextures()->GetRHIHandle(cible->GetColorResult()));

    float32 lacetRad = 0.f;
    float32 tangageRad = 0.f;
    float32 accumMouseX = 0.f;
    float32 accumMouseY = 0.f;

    uint64_t prevNano = NkChrono::Now().nanoseconds;
    bool premiereImage = true;

    NkQuatf prevOrientation = NkQuatf::Identity();
    NkQuatf prevPredOrientation = NkQuatf::Identity();
    bool hasPrevPred = false;

    std::vector<float32> erreursPred;
    std::vector<float32> erreursSansPred;
    int imagesImmobiles = 0;
    int imageCount = 0;
    const int kTargetImages = 1000;
    bool simulationTerminee = false;

    while (fenetre.IsOpen() && !simulationTerminee) {
        NkEvent *ev = nullptr;
        while (NkEvents().PollEvent(ev)) {
            if (ev->As<NkWindowCloseEvent>() != nullptr) {
                simulationTerminee = true;
            }
            if (auto *rawEv = ev->As<NkMouseRawEvent>()) {
                accumMouseX += rawEv->GetDeltaX();
                accumMouseY += rawEv->GetDeltaY();
            }
        }
        if (simulationTerminee) break;

        uint64_t currentNano = NkChrono::Now().nanoseconds;
        float32 dtSeconds = static_cast<float32>(currentNano - prevNano) / 1e9f;
        prevNano = currentNano;
        if (dtSeconds <= 0.f) dtSeconds = 1.f / 60.f;

        // Application de l'accumulation de la souris
        lacetRad -= accumMouseX * 0.0022f;
        tangageRad -= accumMouseY * 0.0022f;

        bool immobile = (accumMouseX == 0.f && accumMouseY == 0.f);
        if (immobile) {
            imagesImmobiles++;
        }
        accumMouseX = 0.f;
        accumMouseY = 0.f;

        const NkMat4f rotation = NkMat4f::RotationY(NkAngle::FromRad(lacetRad)) *
                                 NkMat4f::RotationX(NkAngle::FromRad(tangageRad));
        const NkQuatf currentOrientation = NkQuatf(rotation).Normalized();

        if (premiereImage) {
            prevOrientation = currentOrientation;
            prevPredOrientation = currentOrientation;
            premiereImage = false;
        } else {
            // 1. Calcul de la vitesse angulaire par rapport à l'image précédente
            NkVec3f vitesse = VitesseAngulaire(prevOrientation, currentOrientation, dtSeconds);

            // 2. Erreur sans prédiction : angle entre l'orientation précédente (brute) et la courante mesurée
            float32 errSans = (currentOrientation * prevOrientation.Conjugate()).Normalized().Angle().Deg();
            erreursSansPred.push_back(errSans);

            // 3. Erreur avec prédiction : comparer la prédiction faite à l'image d'avant à l'orientation mesurée actuelle
            if (hasPrevPred) {
                float32 errPred = (currentOrientation * prevPredOrientation.Conjugate()).Normalized().Angle().Deg();
                erreursPred.push_back(errPred);
            }

            // 4. Extrapolation pour la prochaine image
            prevPredOrientation = Extrapoler(currentOrientation, vitesse, dtFrame);
            hasPrevPred = true;

            prevOrientation = currentOrientation;
            imageCount++;

            if (imageCount >= kTargetImages) {
                simulationTerminee = true;
            }
        }

        if (rEcran->BeginFrame()) {
            NkICommandBuffer *cmd = rEcran->GetCmd();
            NkRender3D *r3d = renderer->GetRender3D();

            renderer->FlushGraphRebuilds();
            r3d->ResetFrame();
            if (auto *mc = renderer->GetMaterialCollection()) {
                mc->Upload();
            }

            NkCamera3DData cd;
            cd.position = { 0.30f, kYeux, 1.20f };
            cd.target = cd.position + currentOrientation * NkVec3f(0.f, 0.f, -1.f);
            cd.up = { 0.f, 1.f, 0.f };
            cd.fovY = 70.f;
            cd.aspect = (float32)devInit.width / (float32)devInit.height;
            cd.nearPlane = 0.05f;
            cd.farPlane = 100.f;

            NkSceneContext sctx;
            sctx.camera = NkCamera3D(cd);
            sctx.time = (float32)imageCount / 60.f;
            r3d->SetIBLStrength(0.35f);

            NkLightDesc soleil;
            soleil.type = NkLightType::NK_DIRECTIONAL;
            soleil.direction = { -0.5f, -1.f, -0.35f };
            soleil.color = { 1.f, 0.96f, 0.88f };
            soleil.intensity = 3.f;
            soleil.castShadow = false;
            sctx.lights.PushBack(soleil);

            r3d->BeginScene(sctx);
            SoumettreLaSalle(r3d, renderer->GetMeshSystem());

            if (auto *graph = renderer->GetRenderGraph()) {
                graph->Execute(cmd);
            }

            if (auto *r2d = rEcran->GetRender2D()) {
                r2d->DrawImage(cible->GetColorResult(), { 0.f, 0.f, (float32)devInit.width, (float32)devInit.height });
            }

            rEcran->Present();
            rEcran->EndFrame();
        }
    }

    // Calcul des moyennes et maximums
    float sumPred = 0.f, maxPred = 0.f;
    for (float e : erreursPred) {
        sumPred += e;
        if (e > maxPred) maxPred = e;
    }
    float avgPred = erreursPred.empty() ? 0.f : sumPred / erreursPred.size();

    float sumSans = 0.f, maxSans = 0.f;
    for (float e : erreursSansPred) {
        sumSans += e;
        if (e > maxSans) maxSans = e;
    }
    float avgSans = erreursSansPred.empty() ? 0.f : sumSans / erreursSansPred.size();

    // Écriture du fichier erreurs.txt
    std::ofstream out("erreurs.txt");
    if (out.is_open()) {
        out << "avec prediction : moyenne " << avgPred << " deg, max " << maxPred << " deg, sur " << erreursPred.size() << " images\n";
        out << "sans prediction : moyenne " << avgSans << " deg, max " << maxSans << " deg, sur " << erreursSansPred.size() << " images\n";
        out.close();
    }

    std::printf("[Predire] Simulation terminee. Images immobiles : %d / %d\n", imagesImmobiles, imageCount);
    std::printf("Fichier erreurs.txt genere avec succes.\n");

    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);
    return 0;
}