#include <iostream>
#include <string>
#include <vector>

int main() {
    // Optimisation des flux E/S
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, N;
    if (!(std::cin >> n >> N)) return 0;

    int prochaine = 0;
    bool estEmpruntee = false;
    int imageEmpruntee = -1;

    std::vector<bool> wasDrawn(n, false);
    int releasedSinceLastSubmit = 0;

    int soumises = 0;
    int refus = 0;
    int horsEmprunt = 0;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        if (!(std::cin >> cmd)) break;

        if (cmd == "acquire") {
            if (estEmpruntee) {
                std::cout << "ACQUIRE REFUSE\n";
                refus++;
            } else {
                estEmpruntee = true;
                imageEmpruntee = prochaine;
                wasDrawn[imageEmpruntee] = false; // Réinitialisé lors de l'emprunt
                std::cout << "ACQUIRE " << imageEmpruntee << "\n";
            }
        }
        else if (cmd == "draw") {
            if (estEmpruntee) {
                wasDrawn[imageEmpruntee] = true;
                std::cout << "DRAW " << imageEmpruntee << "\n";
            } else {
                std::cout << "DRAW HORS EMPRUNT\n";
                horsEmprunt++;
            }
        }
        else if (cmd == "release") {
            if (!estEmpruntee) {
                std::cout << "RELEASE REFUSE\n";
                refus++;
            } else {
                estEmpruntee = false;
                prochaine = (prochaine + 1) % n;
                releasedSinceLastSubmit++;
                std::cout << "RELEASE OK\n";
            }
        }
        else if (cmd == "submit") {
            if (estEmpruntee) {
                std::cout << "SUBMIT REFUSE EMPRUNTEE\n";
                refus++;
            } else if (releasedSinceLastSubmit == 0) {
                std::cout << "SUBMIT REFUSE RIEN\n";
                refus++;
            } else {
                int j = (prochaine + n - 1) % n;
                soumises++;
                releasedSinceLastSubmit = 0; // Accepte toutes les libérations en attente

                if (!wasDrawn[j]) {
                    std::cout << "SUBMIT " << j << " VIDE\n";
                } else {
                    std::cout << "SUBMIT " << j << "\n";
                }
            }
        }
    }

    // Bilan final strict
    std::cout << "SOUMISES " << soumises << "\n";
    std::cout << "REFUS " << refus << "\n";
    std::cout << "HORS EMPRUNT " << horsEmprunt << "\n";

    return 0;
}