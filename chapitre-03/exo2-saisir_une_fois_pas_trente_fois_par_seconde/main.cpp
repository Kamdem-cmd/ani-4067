#include <iostream>
#include "NKWindow/NKWindow.h" 
#include "NKEvent/NkWindowEvent.h"
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
    
    bool running = true;
    int compteur = 0;
    nkentseu::NkEventSystem& evenements = nkentseu::NkEvents();

    evenements.AddEventCallback<nkentseu::NkKeyPressEvent>([&]
        (nkentseu::NkKeyPressEvent* e){
            if(e->GetKey() == nkentseu::NkKey::NK_SPACE){
                compteur++;
                std::cout<<"SAISIR.\n";
            }
    });

    evenements.AddEventCallback<nkentseu::NkKeyRepeatEvent>([&]
        (nkentseu::NkKeyRepeatEvent* e){
            if(e->GetKey() == nkentseu::NkKey::NK_SPACE){
                compteur++;
                std::cout<<"RIEN.\n";
            }
    });

    evenements.AddEventCallback<nkentseu::NkKeyReleaseEvent>([]
        (nkentseu::NkKeyReleaseEvent* e){
            if(e->GetKey() == nkentseu::NkKey::NK_SPACE){
                std::cout<<"LACHER.\n";
            }
    });

    evenements.AddEventCallback<nkentseu::NkWindowCloseEvent>([&]
        (nkentseu::NkWindowCloseEvent* e){
            running = false;
    });

    while(running){
        evenements.PollEvents();
    }

    std::cout << "SANS_GARDE " << compteur << "\n";

    return 0;
}