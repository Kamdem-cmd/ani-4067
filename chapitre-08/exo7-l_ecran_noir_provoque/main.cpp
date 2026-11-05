#include <iostream>
#include <string>
#include <vector>

int main() {
    // Optimisation des flux E/S
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    int currentTargetHandle = 100; // La première cible reçoit 101
    int activeTargetHandle = 0;    // Aucune cible active au départ
    uint64_t chainSlot0 = 0;       // L'index 0 de la chaîne vaut 0 au départ

    int imageCount = 0;
    int affichees = 0;
    int noires = 0;
    int perimees = 0;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        if (!(std::cin >> cmd)) break;

        if (cmd == "cible") {
            int w, h;
            std::cin >> w >> h;
            currentTargetHandle++;
            activeTargetHandle = currentTargetHandle;
            std::cout << "CIBLE " << activeTargetHandle << " " << w << " " << h << "\n";
        }
        else if (cmd == "chaine") {
            // Détruit la chaîne et en crée une neuve, d'une image (index 0 remis à 0)
            chainSlot0 = 0;
        }
        else if (cmd == "enregistre") {
            // Range la poignée de la cible courante à l'index 0 de la chaîne
            chainSlot0 = activeTargetHandle;
        }
        else if (cmd == "image") {
            imageCount++;
            uint64_t h = chainSlot0;

            if (h == 0) {
                std::cout << "IMAGE " << imageCount << " NOIR\n";
                noires++;
            } 
            else if (static_cast<int>(h) == activeTargetHandle) {
                std::cout << "IMAGE " << imageCount << " AFFICHE " << h << "\n";
                affichees++;
            } 
            else {
                std::cout << "IMAGE " << imageCount << " PERIMEE " << h << "\n";
                perimees++;
            }
        }
    }

    // Bilan final strict
    std::cout << "AFFICHEES " << affichees << "\n";
    std::cout << "NOIRES " << noires << "\n";
    std::cout << "PERIMEES " << perimees << "\n";

    return 0;
}