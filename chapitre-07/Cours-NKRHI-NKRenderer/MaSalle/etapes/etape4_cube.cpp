// =============================================================================
//  Étape 4 — la troisième dimension : un sol, un cube d'un mètre, une caméra,
//  une lumière.
//
//  Deux renderers, et c'est voulu :
//    - rEcran POSSÈDE l'image : BeginFrame, Present, EndFrame. Il ne fait que
//      du 2D ;
//    - renderer dessine la scène 3D, mais PAS à l'écran : dans une CIBLE HORS
//      ÉCRAN, une texture qu'on peut relire. rEcran recopie ensuite cette
//      texture dans la fenêtre.
//  Ce détour permet de CAPTURER l'image (une fenêtre ne se relit pas), et il
//  prépare le chapitre 9 : deux yeux, donc deux cibles.
//
//  Le cube de NkMeshSystem est UNITAIRE, de demi-étendue 0,5 : une échelle S
//  donne une taille S, et le poser au sol demande de le monter de S/2.
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

#include <cstdio>
#include <cstdlib>

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData &d) { d.appName = "MaSalle"; }
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

namespace {

	const float32 kPiece = 4.00f; // côté du sol, en mètres
	const float32 kYeux = 1.70f;  // hauteur des yeux, debout
	const float32 kDemi = kPiece * 0.5f;

	uint64 EnvU64(const char *nom, uint64 defaut) {
		const char *t = getenv(nom);
		if (t == nullptr || *t == '\0') return defaut;
		return (uint64)strtoull(t, nullptr, 10);
	}

	// Un pavé posé dans le monde. `centre` est le centre géométrique, `taille`
	// les trois dimensions en mètres : on ne manipule jamais autre chose.
	void Pave(NkRender3D *r3d, NkMeshSystem *meshes, const NkVec3f &centre,
			  const NkVec3f &taille, 
			  const NkVec3f &teinte, float32 rugosite = 1.f) {
		NkDrawCall3D dc;
		dc.mesh = meshes->GetCube();
		// On lit de droite à gauche : d'abord l'échelle, puis la translation.
		dc.transform = NkMat4f::Translate(centre) * NkMat4f::Scale(taille);
		// La boîte englobante, dans le monde : le moteur s'en sert pour écarter
		// ce qui est hors du champ. Fausse, l'objet disparaît sans un mot.
		dc.aabb = { { centre.x - taille.x * 0.5f, centre.y - taille.y * 0.5f,
					  centre.z - taille.z * 0.5f },
					{ centre.x + taille.x * 0.5f, centre.y + taille.y * 0.5f,
					  centre.z + taille.z * 0.5f } };
		dc.tint = teinte;
		dc.metallic = 0.f;
		dc.roughness = rugosite;
		r3d->Submit(dc);
	}

	void SoumettreLaScene(NkRender3D *r3d, NkMeshSystem *meshes) {
		{ // le sol : le plan est unitaire, une échelle S donne un côté S
			NkDrawCall3D dc;
			dc.mesh = meshes->GetPlane();
			dc.transform = NkMat4f::Scale({ kPiece, 1.f, kPiece });
			dc.aabb = { { -kDemi, 0.f, -kDemi }, { kDemi, 0.f, kDemi } };
			dc.castShadow = false;
			dc.tint = { 0.38f, 0.36f, 0.34f };
			dc.metallic = 0.f;
			dc.roughness = 1.f;
			r3d->Submit(dc);
		}

		// Le repère d'un mètre : un cube d'un mètre de côté, posé au sol.
		// Son centre est donc à 0,50 m de haut.
		Pave(r3d, meshes, { 0.f, 0.50f, 0.f }, { 1.f, 1.f, 1.f }, { 0.85f, 0.80f, 0.30f }, 0.8f);
	}

} // namespace

int nkmain(const NkEntryState &state) {
	(void)state;

	NkWindowConfig cfg;
	cfg.title = "Ma salle";
	cfg.width = 1280;
	cfg.height = 720;
	cfg.centered = true;
	cfg.vsync = true;

	NkWindow fenetre(cfg);
	if (!fenetre.IsValid()) {
		std::printf("[MaSalle] la fenetre n'a pas pu s'ouvrir\n");
		return 1;
	}

	NkDeviceInitInfo devInit{};
	devInit.surface = fenetre.GetSurfaceDesc();
	devInit.width = fenetre.GetSize().x;
	devInit.height = fenetre.GetSize().y;

	NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);
	if (device == nullptr) {
		std::printf("[MaSalle] aucune API graphique disponible\n");
		return 1;
	}

	// ── 1. Le renderer d'écran : il possède l'image, il ne fait que du 2D ──
	NkRendererConfig cfgEcran =
		NkRendererConfig::For2D(devInit.api, devInit.width, devInit.height);
	NkRenderer *rEcran = NkRenderer::Create(device, cfgEcran);
	if (rEcran == nullptr || !rEcran->Initialize()) {
		std::printf("[MaSalle] le renderer d'ecran n'a pas pu s'initialiser\n");
		NkDeviceFactory::Destroy(device);
		return 1;
	}

	// ── 2. La cible hors écran : une texture où la scène sera dessinée ─────
	NkOffscreenDesc od;
	od.width = devInit.width;
	od.height = devInit.height;
	od.hasDepth = true; // sans profondeur, le fond se dessine par-dessus le devant
	od.hdr = false;
	od.colorFmt = NkGPUFormat::NK_RGBA8_UNORM;
	od.readable = true;
	od.readback = true; // sans quoi Capture() ne peut rien relire
	od.name = "MaSalle_Cible";

	NkOffscreenTarget *cible = rEcran->CreateOffscreen(od);
	if (cible == nullptr) {
		std::printf("[MaSalle] cible hors ecran refusee\n");
		NkRenderer::Destroy(rEcran);
		NkDeviceFactory::Destroy(device);
		return 1;
	}

	// ── 3. Le renderer de la scène : ForGame, moins ce dont on ne se sert pas ──
	NkRendererConfig cfgScene =
		NkRendererConfig::ForGame(devInit.api, devInit.width, devInit.height);
	cfgScene.Disable(NK_SS_RENDER2D);
	cfgScene.Disable(NK_SS_TEXT);
	cfgScene.Disable(NK_SS_OVERLAY);
	cfgScene.Enable(NK_SS_OFFSCREEN);

	NkRenderer *renderer = NkRenderer::Create(device, cfgScene);
	if (renderer == nullptr || !renderer->Initialize()) {
		std::printf("[MaSalle] le renderer de la scene n'a pas pu s'initialiser\n");
		NkRenderer::Destroy(rEcran);
		NkDeviceFactory::Destroy(device);
		return 1;
	}
	// Il dessine DANS la cible. Une fois, ici : pas à chaque image.
	renderer->SetFinalColorTarget(
		rEcran->GetTextures()->GetRHIHandle(cible->GetColorResult()));

	// MASALLE_EXIT=<image> arrête proprement ; MASALLE_SHOT=<image> capture.
	const uint64 imageDeSortie = EnvU64("MASALLE_EXIT", 0);
	const uint64 imageDeCapture = EnvU64("MASALLE_SHOT", 0);

	uint64 image = 0;
	bool tourne = true;

	while (tourne && fenetre.IsOpen()) {
		NkEvent *ev = nullptr;
		while (NkEvents().PollEvent(ev)) {
			if (ev->As<NkWindowCloseEvent>() != nullptr) {
				tourne = false;
			}
		}
		if (!tourne) break;

		if (rEcran->BeginFrame()) {
			NkICommandBuffer *cmd = rEcran->GetCmd();
			NkRender3D *r3d = renderer->GetRender3D();

			// Ce que BeginFrame ferait si ce renderer possédait l'image. En
			// mode partagé, on le rejoue à la main. Oublier ResetFrame piétine
			// les tampons d'une image à l'autre.
			renderer->FlushGraphRebuilds();
			r3d->ResetFrame();
			if (auto *mc = renderer->GetMaterialCollection()) {
				mc->Upload();
			}

			// ── La caméra : où l'on est, ce qu'on regarde, où est le haut ──
			NkCamera3DData cd;
			cd.position = { 2.20f, kYeux, 3.00f };
			cd.target = { 0.f, 0.50f, 0.f }; // le centre du cube
			cd.up = { 0.f, 1.f, 0.f };
			cd.fovY = 70.f;
			cd.aspect = (float32)devInit.width / (float32)devInit.height;
			cd.nearPlane = 0.05f;
			cd.farPlane = 100.f;

			// ── Le contexte de la scène : la caméra, le temps, les lumières ──
			NkSceneContext sctx;
			sctx.camera = NkCamera3D(cd);
			sctx.time = (float32)image / 60.f;

			NkLightDesc soleil;
			soleil.type = NkLightType::NK_DIRECTIONAL;
			soleil.direction = { -0.5f, -1.f, -0.35f };
			soleil.color = { 1.f, 0.96f, 0.88f };
			soleil.intensity = 3.f;
			soleil.castShadow = false;
			sctx.lights.PushBack(soleil);

			r3d->SetIBLStrength(0.35f); // la part de lumière qui vient du ciel
			r3d->BeginScene(sctx);
			SoumettreLaScene(r3d, renderer->GetMeshSystem());

			// Le graphe de la scène s'exécute sur le tampon de commandes de
			// l'écran, AVANT son Present.
			if (auto *graph = renderer->GetRenderGraph()) {
				graph->Execute(cmd);
			}

			// La texture, montrée dans la fenêtre.
			if (auto *r2d = rEcran->GetRender2D()) {
				r2d->DrawImage(cible->GetColorResult(),
							   { 0.f, 0.f, (float32)devInit.width, (float32)devInit.height });
			}

			rEcran->Present();
			rEcran->EndFrame();
		}

		++image;

		if (imageDeCapture != 0 && image == imageDeCapture) {
			const char *chemin = getenv("MASALLE_SHOT_PATH");
			if (chemin == nullptr || *chemin == '\0') {
				chemin = "masalle.png";
			}
			const bool ok = cible->Capture(chemin);
			std::printf("[MaSalle] capture image %llu -> %s (%s)\n",
						(unsigned long long)image, chemin, ok ? "ok" : "echec");
		}

		if (imageDeSortie != 0 && image >= imageDeSortie) {
			tourne = false;
		}
	}

	std::printf("[MaSalle] %llu images\n", (unsigned long long)image);

	NkRenderer::Destroy(renderer);
	NkRenderer::Destroy(rEcran);
	NkDeviceFactory::Destroy(device);
	return 0;
}
