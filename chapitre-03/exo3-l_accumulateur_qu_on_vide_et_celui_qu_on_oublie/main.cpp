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
            if(/*nkentseu::NkMouseRawEvent()*/){
                
            }
        }
    }
    return 0;
}
