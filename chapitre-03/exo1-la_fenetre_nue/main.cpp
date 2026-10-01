#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h" 


int nkmain(const nkentseu::NkEntryState &state) {
    
    nkentseu::NkWindowConfig config;
    config.title  = "Ma salle";
    config.width  = 1280;
    config.height = 720;

    nkentseu::NkWindow fenetre(config);
    if (!fenetre.IsValid()) {
        return 1;
    }

    while (fenetre.IsOpen()) {
        nkentseu::NkEvents().PollEvents();
    }

    return 0;
}
