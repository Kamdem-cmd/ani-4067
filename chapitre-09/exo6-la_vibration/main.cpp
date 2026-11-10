#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstdint>

int main() {
    // Optimisation des flux E/S
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    uint64_t n = 0;
    int N = 0;
    if (!(std::cin >> n >> N)) return 0;

    // Emplacements de la chaîne, initialisés à 0
    std::vector<uint64_t> chaine(n, 0);

    // Ensemble des textures existantes (créées et non détruites)
    std::unordered_set<uint64_t> texturesExistantes;

    int mortesCount = 0;
    int largesCount = 0;

    for (int k = 0; k < N; ++k) {
        std::string cmd;
        if (!(std::cin >> cmd)) break;

        if (cmd == "cree") {
            uint64_t h;
            std::cin >> h;
            texturesExistantes.insert(h);
        }
        else if (cmd == "detruit") {
            uint64_t h;
            std::cin >> h;
            texturesExistantes.erase(h);
        }
        else if (cmd == "set") {
            uint64_t i, h;
            std::cin >> i >> h;
            if (i >= n) {
                std::cout << "SET IGNORE " << i << "\n";
            } else {
                chaine[i] = h;
                if (h > 4294967295ULL) {
                    largesCount++;
                }
            }
        }
        else if (cmd == "get") {
            uint64_t i;
            std::cin >> i;
            if (i >= n) {
                std::cout << "GET " << i << " 0 VIDE\n";
            } else {
                uint64_t h = chaine[i];
                if (h == 0) {
                    std::cout << "GET " << i << " 0 VIDE\n";
                } else if (texturesExistantes.count(h) > 0) {
                    std::cout << "GET " << i << " " << h << " VIVANTE\n";
                } else {
                    std::cout << "GET " << i << " " << h << " MORTE\n";
                    mortesCount++;
                }
            }
        }
    }

    // Bilan final strict
    std::cout << "MORTES " << mortesCount << "\n";
    std::cout << "LARGES " << largesCount << "\n";

    return 0;
}