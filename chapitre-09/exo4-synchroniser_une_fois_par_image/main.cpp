#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T = 0;
    if (!(std::cin >> T)) return 0;

    int current = 0;
    int changed = 0;
    int active = 0;

    int appuisReels = 0;
    int saisies = 0;
    int syncRefusees = 0;
    int pPrev = 0;

    for (int t = 1; t <= T; ++t) {
        int p, s, f;
        std::cin >> p >> s >> f;

        // Suivi des appuis réels (p passe de 0 à 1)
        if (p == 1 && pPrev == 0) {
            appuisReels++;
        }
        pPrev = p;

        int appelsReussis = 0;
        for (int i = 0; i < s; ++i) {
            if (f == 1) {
                appelsReussis++;
                changed = (p != current) ? 1 : 0;
                current = p;
                active = 1;
            } else {
                syncRefusees++;
            }
        }

        std::string saisitStr = "-";
        if (current == 1 && changed == 1) {
            saisitStr = "SAISIT";
            saisies++;
        }

        std::cout << t << " " << current << " " << changed << " " << active << " " << saisitStr << "\n";
    }

    std::cout << "APPUIS REELS " << appuisReels << "\n";
    std::cout << "SAISIES " << saisies << "\n";
    std::cout << "SYNC REFUSEES " << syncRefusees << "\n";

    return 0;
}