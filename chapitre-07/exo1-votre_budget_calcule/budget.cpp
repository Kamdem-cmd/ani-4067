// =============================================================================
//  Exercice : Votre budget calculé (Chapitre 07)
// =============================================================================
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"

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

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData &d) { d.appName = "BudgetVR"; }
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

    uint64 EnvU64(const char *nom, uint64 defaut) {
        const char *t = getenv(nom);
        if (t == nullptr || *t == '\0') return defaut;
        return (uint64)strtoull(t, nullptr, 10);
    }
} // namespace

int nkmain(const NkEntryState &state) {
    (void)state;

    // 2. Calcul et impression des budgets d'une image à 72, 90 et 120 Hz (3 décimales)
    std::printf("budget 72 Hz : %.3f ms\n", 1000.0 / 72.0);
    std::printf("budget 90 Hz : %.3f ms\n", 1000.0 / 90.0);
    std::printf("budget 120 Hz : %.3f ms\n", 1000.0 / 120.0);

    NkWindowConfig cfg;
    cfg.title = "Chapitre 07 - Budget VR";
    cfg.width = 1280;
    cfg.height = 720;
    cfg.centered = true;
    cfg.vsync = true;

    NkWindow fenetre(cfg);
    if (!fenetre.IsValid()) {
        std::printf("[BudgetVR] la fenetre n'a pas pu s'ouvrir\n");
        return 1;
    }

    NkDeviceInitInfo devInit{};
    devInit.surface = fenetre.GetSurfaceDesc();
    devInit.width = fenetre.GetSize().x;
    devInit.height = fenetre.GetSize().y;

    NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);
    if (device == nullptr) {
        std::printf("[BudgetVR] aucune API graphique disponible\n");
        return 1;
    }

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
    od.hdr = false;
    od.colorFmt = NkGPUFormat::NK_RGBA8_UNORM;
    od.readable = true;
    od.readback = true;
    od.name = "BudgetVR_Cible";

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

    const uint64 imageDeSortie = EnvU64("MASALLE_EXIT", 1040); // 30 écartées + 1000 mesurées

    uint64 image = 0;
    bool tourne = true;

    // Variables pour la mesure des intervalles d'images
    uint64_t lastNanoTime = 0;
    std::vector<double> intervals;
    intervals.reserve(1000);

    while (tourne && fenetre.IsOpen()) {
        NkEvent *ev = nullptr;
        while (NkEvents().PollEvent(ev)) {
            if (ev->As<NkWindowCloseEvent>() != nullptr) {
                tourne = false;
            }
        }
        if (!tourne) break;

        // 3. Noter l'instant présent en nanosecondes au début de chaque image via NkChrono::Now().nanoseconds
        uint64_t currentNanoTime = NkChrono::Now().nanoseconds;

        if (image > 0 && image >= 30 && image < 30 + 1000) {
            uint64_t diffNano = currentNanoTime - lastNanoTime;
            double diffMs = (double)diffNano / 1'000'000.0;
            intervals.push_back(diffMs);
        }
        lastNanoTime = currentNanoTime;

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
            cd.target = { -0.70f, 1.10f, -kDemi };
            cd.up = { 0.f, 1.f, 0.f };
            cd.fovY = 70.f;
            cd.aspect = (float32)devInit.width / (float32)devInit.height;
            cd.nearPlane = 0.05f;
            cd.farPlane = 100.f;

            NkSceneContext sctx;
            sctx.camera = NkCamera3D(cd);
            sctx.time = (float32)image / 60.f;
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

        ++image;

        if (imageDeSortie != 0 && image >= imageDeSortie) {
            tourne = false;
        }
    }

    // 5. Calcul et impression de la moyenne, médiane et max en millisecondes au format demandé
    if (!intervals.empty()) {
        double sum = std::accumulate(intervals.begin(), intervals.end(), 0.0);
        double moyenne = sum / intervals.size();

        std::sort(intervals.begin(), intervals.end());
        double mediane = 0.0;
        size_t size = intervals.size();
        if (size % 2 == 0) {
            mediane = (intervals[size / 2 - 1] + intervals[size / 2]) / 2.0;
        } else {
            mediane = intervals[size / 2];
        }
        double maxVal = intervals.back();

        std::printf("ecran : moyenne %.3f ms, mediane %.3f ms, max %.3f ms\n", moyenne, mediane, maxVal);
    }

    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);
    return 0;
}