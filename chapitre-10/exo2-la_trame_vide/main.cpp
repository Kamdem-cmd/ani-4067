#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0, T = 0;
    if (!(std::cin >> N >> T)) return 0;

    if (N < 1 || N > 4) {
        std::cout << "CHAINE REFUSEE\n";
        return 0;
    }

    std::string signes;
    if (T > 0) {
        std::cin >> signes;
    }

    int prochaine = 0;
    int fins = 0;
    int rendues = 0;
    int sautees = 0;

    for (int t = 1; t <= T; ++t) {
        fins++;
        int lacet = t - 1;
        if (signes[t - 1] == '1') {
            int imgIndex = prochaine;
            prochaine = (prochaine + 1) % N;
            rendues++;
            std::cout << t << " OUI 1 " << lacet << " " << imgIndex << "\n";
        } else {
            sautees++;
            std::cout << t << " NON 0\n";
        }
    }

    std::cout << "FINS " << fins << "\n";
    std::cout << "RENDUES " << rendues << "\n";
    std::cout << "SAUTEES " << sautees << "\n";
    std::cout << "PROCHAINE " << prochaine << "\n";

    return 0;
}