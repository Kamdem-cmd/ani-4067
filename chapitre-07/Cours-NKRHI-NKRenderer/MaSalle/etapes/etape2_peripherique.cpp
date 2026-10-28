// =============================================================================
//  Étape 2 — le périphérique : NKRHI parle à la carte graphique.
//
//  La fenêtre de l'étape 1, plus TROIS lignes : on décrit ce qu'on veut
//  (NkDeviceInitInfo), on demande un périphérique (NkDeviceFactory), on le
//  rend à la fin. Rien ne se dessine encore : on vérifie que la carte répond,
//  et on lit QUELLE API le moteur a retenue sur cette machine.
// =============================================================================
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"

#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRHI/Core/NkGraphicsApi.h"

#include <cstdio>
#include <cstdlib>

using namespace nkentseu;

static void ConfigureAppData(NkAppData &d) { d.appName = "MaSalle"; }
NK_REGISTER_ENTRY_APPDATA_UPDATER(ConfigureAppData)

namespace {

	// Une variable d'environnement lue comme un entier. Elle sert à arrêter le
	// programme tout seul (MASALLE_EXIT=<image>) : la construction se vérifie
	// alors sans main humaine.
	uint64 EnvU64(const char *nom, uint64 defaut) {
		const char *t = getenv(nom);
		if (t == nullptr || *t == '\0') return defaut;
		return (uint64)strtoull(t, nullptr, 10);
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

	// ── Le périphérique ────────────────────────────────────────────────────
	// 1. Ce qu'on veut : dessiner DANS cette fenêtre, à cette taille.
	NkDeviceInitInfo devInit{};
	devInit.surface = fenetre.GetSurfaceDesc();
	devInit.width = fenetre.GetSize().x;
	devInit.height = fenetre.GetSize().y;

	// 2. Le moteur essaie les API de la machine et garde la première qui
	//    répond. Il ÉCRIT son choix dans devInit.api : c'est pour cela que la
	//    fonction prend devInit par référence, et non par copie.
	NkIDevice *device = NkDeviceFactory::CreateAutoDetect(devInit);
	if (device == nullptr) {
		std::printf("[MaSalle] aucune API graphique disponible\n");
		return 1;
	}

	std::printf("[MaSalle] API graphique retenue : %s, surface de %ux%u\n", NkGraphicsApiName(devInit.api), devInit.width, devInit.height");

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

		++image;
		if (imageDeSortie != 0 && image >= imageDeSortie) {
			tourne = false;
		}
	}

	std::printf("[MaSalle] %llu images\n", (unsigned long long)image);

	// 3. Ce qu'on a demandé, on le rend. Dans l'ordre inverse de la création.
	NkDeviceFactory::Destroy(device);
	return 0;
}
