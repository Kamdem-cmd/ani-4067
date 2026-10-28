// =============================================================================
//  Étape 3 — le renderer : NKRenderer dessine, d'abord en 2D.
//
//  Le périphérique de l'étape 2, plus un NkRenderer configuré pour le 2D. On
//  y trace le PLAN de la salle vue de dessus, aux dimensions du chapitre 2 :
//  avant de bâtir un lieu en trois dimensions, on le dessine à plat.
//
//  La boucle de rendu tient en quatre appels, toujours dans cet ordre :
//      BeginFrame  ->  on dessine  ->  Present  ->  EndFrame
// =============================================================================
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"

#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRHI/Core/NkGraphicsApi.h"
#include "NKRenderer/NkRenderer.h"
#include "NKRenderer/Tools/Render2D/NkRender2D.h"

#include <cstdio>
#include <cstdlib>

using namespace nkentseu;
using namespace nkentseu::renderer;

static void ConfigureAppData(NkAppData &d) { d.appName = "MaSalle"; }
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

namespace {

	// ── Les dimensions de la salle, en MÈTRES, en un seul endroit ─────────
	const float32 kPiece = 4.00f;  // côté intérieur du sol
	const float32 kPorteL = 0.90f; // porte : largeur
	const float32 kFenL = 1.20f;   // fenêtre : largeur
	const float32 kTableL = 1.20f; // table : longueur
	const float32 kTableP = 0.80f; //         profondeur
	const float32 kDemi = kPiece * 0.5f;

	uint64 EnvU64(const char *nom, uint64 defaut) {
		const char *t = getenv(nom);
		if (t == nullptr || *t == '\0') return defaut;
		return (uint64)strtoull(t, nullptr, 10);
	}

	// Le plan de la salle, vue de dessus. Le monde est en mètres, l'écran en
	// pixels : `ppm` (pixels par mètre) est l'ÉCHELLE, et (cx, cy) le point de
	// l'écran où tombe le centre de la pièce. X du monde va vers la droite,
	// Z du monde vers le bas de l'écran (vers nous).
	void DessinerLePlan(NkRender2D *r2d, float32 W, float32 H) {
		const float32 ppm = 120.f;
		const float32 cx = W * 0.5f, cy = H * 0.5f;
		const float32 cote = kPiece * ppm;

		const NkVec4f fondHaut = { 0.10f, 0.12f, 0.16f, 1.f };
		const NkVec4f fondBas = { 0.03f, 0.04f, 0.06f, 1.f };
		const NkVec4f sol = { 0.38f, 0.36f, 0.34f, 1.f };
		const NkVec4f mur = { 0.72f, 0.70f, 0.66f, 1.f };
		const NkVec4f bois = { 0.55f, 0.38f, 0.22f, 1.f };
		const NkVec4f vitre = { 0.75f, 0.85f, 0.92f, 1.f };
		const NkVec4f vous = { 0.95f, 0.80f, 0.20f, 1.f };

		// Le fond, puis le sol, puis les murs (un contour épais).
		r2d->FillRectGradV({ 0.f, 0.f, W, H }, fondHaut, fondBas);
		r2d->FillRect({ cx - cote * 0.5f, cy - cote * 0.5f, cote, cote }, sol);
		r2d->DrawRect({ cx - cote * 0.5f, cy - cote * 0.5f, cote, cote }, mur, 8.f);

		// La porte, dans le mur du fond (z = -kDemi), centrée en x = -0,70 m.
		r2d->FillRect({ cx + (-0.70f - kPorteL * 0.5f) * ppm, cy - kDemi * ppm - 6.f,
						kPorteL * ppm, 12.f },
					  bois);

		// Deux fenêtres, dans le mur de gauche (x = -kDemi), en z = -0,90 et +0,90.
		r2d->FillRect({ cx - kDemi * ppm - 6.f, cy + (-0.90f - kFenL * 0.5f) * ppm, 12.f,
						kFenL * ppm },
					  vitre);
		r2d->FillRect({ cx - kDemi * ppm - 6.f, cy + (0.90f - kFenL * 0.5f) * ppm, 12.f,
						kFenL * ppm },
					  vitre);

		// La table : 1,2 m x 0,8 m, centrée en (0,60 ; 0,70).
		r2d->FillRect({ cx + (0.60f - kTableL * 0.5f) * ppm, cy + (0.70f - kTableP * 0.5f) * ppm,
						kTableL * ppm, kTableP * ppm },
					  bois);

		// Vous, debout en (0,30 ; 1,20), et la direction du regard : vers la porte.
		const NkVec2f moi = { cx + 0.30f * ppm, cy + 1.20f * ppm };
		const NkVec2f porte = { cx - 0.70f * ppm, cy - kDemi * ppm };
		r2d->DrawLine(moi, porte, vous, 2.f);
		r2d->FillCircle(moi, 9.f, vous);
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
	std::printf("[MaSalle] API graphique retenue : %s\n", NkGraphicsApiName(devInit.api));

	// ── Le renderer ────────────────────────────────────────────────────────
	// Une CONFIGURATION d'abord : For2D n'allume que ce qu'il faut pour
	// dessiner à plat (formes, images, texte). Chaque sous-système allumé
	// coûte de la mémoire et du temps de démarrage : on ne prend que le sien.
	NkRendererConfig cfgEcran =
		NkRendererConfig::For2D(devInit.api, devInit.width, devInit.height);
	NkRenderer *rEcran = NkRenderer::Create(device, cfgEcran);
	if (rEcran == nullptr || !rEcran->Initialize()) {
		std::printf("[MaSalle] le renderer n'a pas pu s'initialiser\n");
		NkDeviceFactory::Destroy(device);
		return 1;
	}

	const uint64 imageDeSortie = EnvU64("MASALLE_EXIT", 0);
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

		// BeginFrame peut refuser (fenêtre réduite, surface perdue) : dans ce
		// cas on ne dessine pas, et on ne présente pas.
		if (rEcran->BeginFrame()) {
			if (auto *r2d = rEcran->GetRender2D()) {
				DessinerLePlan(r2d, (float32)devInit.width, (float32)devInit.height);
			}
			rEcran->Present();
			rEcran->EndFrame();
		}

		++image;
		if (imageDeSortie != 0 && image >= imageDeSortie) {
			tourne = false;
		}
	}

	std::printf("[MaSalle] %llu images\n", (unsigned long long)image);

	// Dans l'ordre inverse de la création : le renderer, puis le périphérique.
	NkRenderer::Destroy(rEcran);
	NkDeviceFactory::Destroy(device);
	return 0;
}
