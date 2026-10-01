#include <iostream>
#include "NKWindow/NKWindow.h" 
#include "NKEvent/NkWindowEvent.h"

int main(){
    bool running = true;
    int compteur = 0;

    while(running){
        nkentseu::NkEvent* event = nullptr;
        while (((event = nkentseu::NkEvents().PollEvent()) != nullptr)){
            if (event->Is<nkentseu::NkWindowCloseEvent>()){
                std::cout<<"Dernière ligne : SANS_GARDE 4.\n";
                running = false;
            }

            if(/*evenement de saisie*/){
                compteur++;
                std::cout<<"enfonce : SAISIR. Le compteur sans garde passe à " << compteur << "\n";
            }else if(/*evenement de maintenue*/){
                compteur++;
                std::cout<<"repete : RIEN. Le compteur passe à " << compteur << "\n";
            }else if(/*evenement de relache*/){
                std::cout<<"relache : LACHER.\n";
            }
        }
    }
    return 0;
}
