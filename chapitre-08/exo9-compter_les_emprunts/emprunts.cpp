#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0, N = 0;
    if (!(std::cin >> n >> N)) return 0;

    bool estEmpruntee = false;
    int reussies = 0;
    int refusees = 0;
    int liberations = 0;
    int premiereFuite = -1;
    bool enFuite = false;

    for (int t = 1; t <= N; ++t) {
        std::string mode;
        std::cin >> mode;

        if (enFuite) {
            // Après une seule fuite, plus aucune trame ne dessine (chaîne figée/bloquée)
            std::cout << t << " BLOQUEE\n";
            continue;
        }

        // Chaque trame commence par un emprunt
        if (estEmpruntee) {
            // L'emprunt est refusé car une image est encore empruntée
            std::cout << t << " BLOQUEE\n";
            refusees++;
        } else {
            // Emprunt réussi
            estEmpruntee = true;
            reussies++;

            if (mode == "normal") {
                // Dessine puis rend l'image
                estEmpruntee = false;
                liberations++;
                std::cout << t << " OK\n";
            }
            else if (mode == "sortie") {
                // Sort aussitôt après l'emprunt sans rendre l'image (fuite)
                std::cout << t << " FUITE\n";
                enFuite = true;
                if (premiereFuite == -1) {
                    premiereFuite = t;
                }
            }
            else if (mode == "double") {
                // Tente un second emprunt qui est refusé, puis rend l'image
                refusees++; // Le second emprunt refusé se compte
                estEmpruntee = false;
                liberations++;
                std::cout << t << " DOUBLE\n";
            }
        }
    }

    int ecart = reussies - liberations;

    std::cout << "REUSSIES " << reussies << "\n";
    std::cout << "REFUSEES " << refusees << "\n";
    std::cout << "LIBERATIONS " << liberations << "\n";
    std::cout << "ECART " << ecart << "\n";

    if (premiereFuite != -1) {
        std::cout << "PREMIERE FUITE " << premiereFuite << "\n";
    } else {
        std::cout << "PREMIERE FUITE AUCUNE\n";
    }

    return 0;
}