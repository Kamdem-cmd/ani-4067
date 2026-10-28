// =============================================================================
//  Étape 1 — une fenêtre qui s'ouvre, et qui se ferme proprement.
//
//  Rien de plus. Pas de rendu, pas de casque, pas de tête : on vérifie que la
//  chaîne complète tient debout, du fichier de projet jusqu'à un rectangle
//  affiché par le système.
//
//  L'usage est celui que documente l'en-tête de NkWindow.h, recopié tel quel :
//      NkInitialise(); NkWindow w(cfg); while (w.IsOpen()) NkEvents().PollEvents();
//      NkClose();
// =============================================================================
#include "NKWindow/NKMain.h"
#include "NKWindow/Core/NkWindow.h"
#include "NKWindow/Core/NkWindowConfig.h"
#include "NKEvent/NkEventSystem.h"

#include <cstdio>

using namespace nkentseu;

int nkmain(const NkEntryState &state) {
	(void)state;

	NkInitialise();

	NkWindowConfig cfg;
	cfg.title = "Ma salle";
	cfg.width = 1280;
	cfg.height = 720;

	NkWindow fenetre(cfg);

	// Une fenêtre qui n'a pas pu s'ouvrir existe quand même comme objet, et ne
	// le dit que si on le lui demande. Sauter ce test donne un programme qui
	// tourne sans rien afficher.
	if (!fenetre.IsValid()) {
		std::printf("[MaSalle] la fenetre n'a pas pu s'ouvrir\n");
		NkClose();
		return 1;
	}

	std::printf("[MaSalle] fenetre ouverte : %ux%u\n", cfg.width, cfg.height);

	// La pompe. Sans elle, le systeme declare la fenetre bloquee, meme si la
	// boucle tourne a plein regime.
	uint64 images = 0;
	while (fenetre.IsOpen()) {
		NkEvents().PollEvents();
		++images;
	}

	std::printf("[MaSalle] fermee proprement apres %llu images\n",
				(unsigned long long)images);

	NkClose();
	return 0;
}
