#include <iostream>
#include "NKWindow/NKWindow.h" 
#include "NKEvent/NkWindowEvent.h"

int main(){
    bool running = true;
    int compteur = 0;
    nkentseu::NkEventSystem& evenements = nkentseu::NkEvents();

    while(running){
        evenements.AddEventCallback<nkentseu::NkKeyPressEvent>([&]
            (nkentseu::NkKeyPressEvent* e){
                if(e->GetKey() == nkentseu::NkKey::NK_SPACE){
                    compteur++;
                    std::cout<<"enfonce : SAISIR. Le compteur sans garde passe à " << compteur << "\n";
                }
        });

        evenements.AddEventCallback<nkentseu::NkKeyRepeatEvent>([&]
            (nkentseu::NkKeyRepeatEvent* e){
                if(e->GetKey() == nkentseu::NkKey::NK_SPACE){
                    compteur++;
                    std::cout<<"repete : RIEN. Le compteur passe à " << compteur << "\n";
                }
        });

        evenements.AddEventCallback<nkentseu::NkKeyReleaseEvent>([]
            (nkentseu::NkKeyReleaseEvent* e){
                if(e->GetKey() == nkentseu::NkKey::NK_SPACE){
                    std::cout<<"relache : LACHER.\n";
                }
        });

        // evenements.AddEventCallback<nkentseu::NkWindowCloseEvent>([&]
        //     (nkentseu::NkWindowCloseEvent* ){
        //         if(e->GetKey() == nkentseu::NkKey::NK_SPACE){
        //             std::cout<<"relache : LACHER.\n";
        //         }
        // });
    }
    return 0;
}
