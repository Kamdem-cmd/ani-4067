#include <iostream>
#include <string>
#include <vector>
#include <sstream>

struct CibleInfo {
    int id;
    int w;
    int h;
};

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;

    std::stringstream ss(line);
    int windowW, windowH, F, N;
    if (!(ss >> windowW >> windowH >> F >> N)) return 0;

    int currentRenderW = windowW;
    int currentRenderH = windowH;
    bool offscreenOn = false;
    int currentTarget = 0; // 0 = fenêtre

    std::vector<CibleInfo> cibles;
    std::vector<bool> bufferBusy(F, false); // true si le tampon porte un travail jamais terminé

    int imageCount = 0;
    bool bloque = false;
    int bloqueIndex = -1;

    for (int i = 0; i < N; ++i) {
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) {
            --i;
            continue;
        }

        if (bloque) {
            // Après un BLOQUE, plus aucun ordre n'agit ni n'affiche, jusqu'au bilan
            continue;
        }

        std::stringstream cmdStream(line);
        std::string cmd;
        cmdStream >> cmd;

        if (cmd == "allume") {
            std::string sub;
            cmdStream >> sub;
            if (sub == "OFFSCREEN") {
                offscreenOn = true;
            }
        }
        else if (cmd == "cible") {
            int w, h;
            cmdStream >> w >> h;
            if (!offscreenOn) {
                std::cout << "CIBLE NULLE\n";
            } else {
                int newId = (int)cibles.size() + 1;
                cibles.push_back({newId, w, h});
                std::cout << "CIBLE " << newId << " " << w << " " << h << "\n";
            }
        }
        else if (cmd == "vers") {
            int k;
            cmdStream >> k;
            if (k == 0) {
                currentTarget = 0;
            } else {
                bool found = false;
                for (const auto& c : cibles) {
                    if (c.id == k) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    currentTarget = k;
                } else {
                    std::cout << "VERS REFUSE\n";
                }
            }
        }
        else if (cmd == "taille") {
            int w, h;
            cmdStream >> w >> h;
            currentRenderW = w;
            currentRenderH = h;
        }
        else if (cmd == "image") {
            imageCount++;
            int bufIndex = (imageCount - 1) % F;

            if (bufferBusy[bufIndex]) {
                bloque = true;
                bloqueIndex = imageCount;
                std::cout << "BLOQUE " << bloqueIndex << "\n";
                break;
            } else {
                // Affichage de l'image
                if (currentTarget == 0) {
                    std::cout << "IMAGE " << imageCount << " FENETRE\n";
                } else {
                    std::cout << "IMAGE " << imageCount << " CIBLE " << currentTarget << "\n";
                }

                // Vérification du blocage (règle 9) :
                // "Si l'on dessine vers une cible et que la zone est plus large OU plus haute qu'elle, 
                //  le travail de l'image ne se termine jamais : son tampon reste pris pour toujours."
                if (currentTarget != 0) {
                    int targetW = 0, targetH = 0;
                    for (const auto& c : cibles) {
                        if (c.id == currentTarget) {
                            targetW = c.w;
                            targetH = c.h;
                            break;
                        }
                    }
                    if (currentRenderW > targetW || currentRenderH > targetH) {
                        bufferBusy[bufIndex] = true;
                    }
                }
            }
        }
    }

    std::cout << "IMAGES " << imageCount << "\n";
    if (bloque) {
        std::cout << "ETAT BLOQUE\n";
    } else {
        std::cout << "ETAT LIBRE\n";
    }

    return 0;
}