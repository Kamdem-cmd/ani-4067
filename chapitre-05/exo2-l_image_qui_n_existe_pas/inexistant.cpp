#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkClock.h"
#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRHI/Core/NkGraphicsApi.h"
#include "NKRenderer/NkRenderer.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKRenderer/Core/NkRenderGraph.h"
#include "NKRenderer/Core/NkTextureLibrary.h"
#include "NKRenderer/Materials/NkMaterialCollection.h"
#include "NKRenderer/Materials/NkMaterial.h"
#include "NKRenderer/Mesh/NkMeshSystem.h"
#include "NKRenderer/Tools/Offscreen/NkOffscreenTarget.h"
#include "NKRenderer/Tools/Render2D/NkRender2D.h"
#include "NKRenderer/Tools/Render3D/NkRender3D.h"
#include "NKImage/Core/NkImage.h"
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::renderer;

NKENTSEU_DEFINE_APP_DATA(([](){
    nkentseu::NkAppData d{};
    d.appName = "ImageInexistante";
    d.appVersion = "1.0.0";
    return d;
})());

int nkmain(const NkEntryState &state) {
    // 1 & 2. Test NkImage sur un chemin faux (tient en 5 lignes max)
    NkImage img;
    bool c = img.Load("murs/nexistepas.png");
    std::printf("charge : %d, valide : %d, largeur : %d, hauteur : %d\n", 
                c, img.IsValid(), img.Width(), img.Height());

    NkWindowConfig cfg;
    cfg.title = "Chapitre 05 - Image Inexistante";
    cfg.width = 1280;
    cfg.height = 720;

    NkWindow window;
    if (!window.Create(cfg)) {
        logger.Error("La fenetre n'a pas ete creee");
        return -1;
    }

    NkDeviceInitInfo devInit{};
    devInit.surface = window.GetSurfaceDesc();
    devInit.width = window.GetSize().x;
    devInit.height = window.GetSize().y;

    NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);
    if (device == nullptr) {
        logger.Error("Echec de creation du device");
        return -2;
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
    od.name = "Render_Cible";

    NkOffscreenTarget *cible = rEcran->CreateOffscreen(od);
    if (cible == nullptr) {
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    NkRendererConfig cfgScene = NkRendererConfig::ForGame(devInit.api, devInit.width, devInit.height);
    cfgScene.Disable(NK_SS_RENDER2D);
    cfgScene.Disable(NK_SS_TEXT);
    cfgScene.Disable(NK_SS_OVERLAY);
    cfgScene.Enable(NK_SS_OFFSCREEN);

    NkRenderer *renderer = NkRenderer::Create(device, cfgScene);
    if (renderer == nullptr || !renderer->Initialize()) {
        NkRenderer::Destroy(rEcran);
        NkDeviceFactory::Destroy(device);
        return 1;
    }

    renderer->SetFinalColorTarget(rEcran->GetTextures()->GetRHIHandle(cible->GetColorResult()));

    // 3. Chargement de la texture absente et attribution au matériau du mur
    auto textureHandle = renderer->GetTextures()->Load("murs/nexistepas.png");
    auto mat = NkMaterial::Create(renderer->GetMaterials(), NkMaterialType::NK_PBR_METALLIC);
    if (mat) {
        mat->SetAlbedoMap(textureHandle);
    }

    const float32 kPiece = 4.00f;
    const float32 kYeux = 1.70f;
    const float32 kDemi = kPiece * 0.5f;
    const float32 kPlafond = 2.50f;
    const float32 kMur = 0.10f;
    const float32 yMur = kPlafond * 0.5f;
    const float32 bord = kDemi + kMur * 0.5f;

    auto &eventSystem = NkEvents();
    bool running = true;
    NkClock horloge;

    while (running) {
        NkEvent *event;
        while (eventSystem.PollEvent(event)) {
            if (event->Is<NkWindowCloseEvent>()) {
                running = false;
            }
        }

        if (!running) break;

        const NkClock::NkTime &temps = horloge.Tick();

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
            cd.target = { -0.7f, 1.10f, -kDemi };
            cd.up = { 0.f, 1.f, 0.f };
            cd.fovY = 70.f;
            cd.aspect = (float32)devInit.width / (float32)devInit.height;
            cd.nearPlane = 0.05f;
            cd.farPlane = 100.f;

            NkSceneContext sctx;
            sctx.camera = NkCamera3D(cd);
            sctx.time = temps.total;

            NkLightDesc soleil;
            soleil.type = NkLightType::NK_DIRECTIONAL;
            soleil.direction = { -0.5f, -1.f, -0.35f };
            soleil.color = { 1.f, 0.96f, 0.88f };
            soleil.intensity = 3.f;
            soleil.castShadow = false;

            sctx.lights.PushBack(soleil);
            r3d->SetIBLStrength(0.35f);
            r3d->BeginScene(sctx);

            // Soumission d'un mur utilisant la texture absente
            {
                NkDrawCall3D dc;
                dc.mesh = renderer->GetMeshSystem()->GetCube();
                const NkVec3f centre = { 0.f, yMur, -bord };
                const NkVec3f taille = { kPiece, kPlafond, kMur };
                dc.transform = NkMat4f::Translate(centre) * NkMat4f::Scale(taille);
                dc.aabb = { { centre.x - taille.x * 0.5f, centre.y - taille.y * 0.5f, centre.z - taille.z * 0.5f },
                            { centre.x + taille.x * 0.5f, centre.y + taille.y * 0.5f, centre.z + taille.z * 0.5f } };
                dc.tint = { 0.72f, 0.70f, 0.66f };
                dc.metallic = 0.f;
                dc.roughness = 1.f;
                if (mat) {
                    dc.material = mat->GetInstHandle();
                }
                r3d->Submit(dc);
            }

            if (auto *graph = renderer->GetRenderGraph()) {
                graph->Execute(cmd);
            }

            if (auto *r2d = rEcran->GetRender2D()) {
                r2d->Begin(cmd, devInit.width, devInit.height);
                r2d->DrawImage(cible->GetColorResult(), { 0.f, 0.f, (float32)devInit.width, (float32)devInit.height });
                r2d->End();
            }

            rEcran->Present();
            rEcran->EndFrame();
        }
    }

    NkRenderer::Destroy(renderer);
    NkRenderer::Destroy(rEcran);
    NkDeviceFactory::Destroy(device);
    return 0;
}