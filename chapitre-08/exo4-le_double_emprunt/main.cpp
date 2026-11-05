#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

int main() {
    // Optimisation des flux E/S
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    uint64_t n = 0, v = 0;
    int N = 0;
    if (!(std::cin >> n >> v >> N)) return 0;

    // 26 variables (de 'a' à 'z') initialisées à v
    std::vector<uint64_t> vars(26, v);

    uint64_t prochaine = 0;
    bool estEmpruntee = false;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        if (!(std::cin >> cmd)) break;

        if (cmd == "acquire") {
            char xChar;
            std::cin >> xChar;
            int idx = xChar - 'a';

            if (estEmpruntee) {
                // Emprunt refusé : la variable X n'est pas modifiée
                std::cout << "ACQUIRE FAUX " << xChar << " " << vars[idx] << "\n";
            } else {
                // Emprunt réussi : on note l'emprunt et on écrit prochaine dans X
                estEmpruntee = true;
                vars[idx] = prochaine;
                std::cout << "ACQUIRE VRAI " << xChar << " " << vars[idx] << "\n";
            }
        }
        else if (cmd == "release") {
            if (!estEmpruntee) {
                std::cout << "RELEASE FAUX\n";
            } else {
                estEmpruntee = false;
                prochaine = (prochaine + 1) % n;
                std::cout << "RELEASE VRAI\n";
            }
        }
        else if (cmd == "last") {
            uint64_t j = (prochaine + n - 1) % n;
            std::cout << "LAST " << j << "\n";
        }
    }

    // Bilan final strict
    if (estEmpruntee) {
        std::cout << "EMPRUNTEE OUI\n";
    } else {
        std::cout << "EMPRUNTEE NON\n";
    }
    std::cout << "PROCHAINE " << prochaine << "\n";

    return 0;
}