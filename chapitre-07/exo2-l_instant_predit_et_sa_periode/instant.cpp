// =============================================================================
//  Exercice : L'instant prédit et sa période (Chapitre 07)
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

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData &d) { d.appName = "InstantPredit"; }
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

    // 2 & 3. Lecture de MASALLE_HZ (72 par défaut) et calcul de la période en ns
    const uint64 hzInput = EnvU64("MASALLE_HZ", 120);
    const double displayHz = (hzInput > 0) ? static_cast<double>(hzInput) : 120.0;
    const double displayPeriodNs = 1e9 / displayHz;
    const double displayPeriodMs = 1000.0 / displayHz;

    NkWindowConfig cfg;
    cfg.title = "Chapitre 07 - Instant Predit";
    cfg.width = 1280;
    cfg.height = 720;
    cfg.centered = true;
    cfg.vsync = true;

    NkWindow fenetre(cfg);
    if (!fenetre.IsValid()) {
        std::printf("[InstantPredit] la fenetre n'a pas pu s'ouvrir\n");
        return 1;
    }

    NkDeviceInitInfo devInit{};
    devInit.surface = fenetre.GetSurfaceDesc();
    devInit.width = fenetre.GetSize().x;
    devInit.height = fenetre.GetSize().y;

    NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);
    if (device == nullptr) {
        std::printf("[InstantPredit] aucune API graphique disponible\n");
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
    od.name = "InstantPredit_Cible";

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

    const uint64 imageDeSortie = EnvU64("MASALLE_EXIT", 700);

    uint64 image = 0;
    bool tourne = true;

    // Horloges en nanosecondes (float64/uint64)
    const uint64_t tLaunchNano = NkChrono::Now().nanoseconds;
    double prevPredictedDisplayNs = 0.0;
    std::vector<double> errorsMs;
    errorsMs.reserve(600);

    while (tourne && fenetre.IsOpen()) {
        NkEvent *ev = nullptr;
        while (NkEvents().PollEvent(ev)) {
            if (ev->As<NkWindowCloseEvent>() != nullptr) {
                tourne = false;
            }
        }
        if (!tourne) break;

        // 4. Instant présent au début de chaque image
        const uint64_t currentNano = NkChrono::Now().nanoseconds;
        const double nowRelNs = static_cast<double>(currentNano - tLaunchNano);
        const double predictedDisplayNs = nowRelNs + displayPeriodNs;

        // 6. Calcul de (reel - predit) à partir de l'image précédente
        if (image > 0) {
            const double errorNs = nowRelNs - prevPredictedDisplayNs;
            const double errorMs = errorNs / 1e6;

            // 7. Consigner pour la moyenne sur 600 images (les 30 premières écartées)
            if (image >= 30 && image < 30 + 600) {
                errorsMs.push_back(errorMs);
            }
        }

        // 5. Pour 20 images de suite (de l'image 30 à 49 par exemple), imprimer la ligne au format demandé
        if (image >= 30 && image < 50) {
            const double nowMs = nowRelNs / 1e6;
            const double predMs = predictedDisplayNs / 1e6;
            const double ecartMs = displayPeriodMs;
            std::printf("maintenant = %.3f ms, predit = %.3f ms, ecart = %.3f ms\n",
                        nowMs, predMs, ecartMs);
        }

        prevPredictedDisplayNs = predictedDisplayNs;

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

    // 7. Impression de la ligne récapitulative
    if (!errorsMs.empty()) {
        const double avgError = std::accumulate(errorsMs.begin(), errorsMs.end(), 0.0) / errorsMs.size();
        std::printf("hz = %llu, periode = %.3f ms, reel - predit = %.3f ms\n",
                    (unsigned long long)hzInput, displayPeriodMs, avgError);
    }

    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);
    return 0;
}