#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long w = 0;
    int T = 0;
    if (!(std::cin >> w >> T)) return 0;

    long long fausses = 0;
    long long pireCasque = 0;
    long long pireSimu = 0;

    for (int t = 1; t <= T; ++t) {
        long long P, S, A;
        char M;
        std::cin >> P >> S >> A >> M;

        long long lacetRendu = w * P;
        long long poseSoumise = (M == 'R') ? (w * P) : (w * S);

        long long casque = poseSoumise - lacetRendu;
        long long simulateur = (w * A) - lacetRendu;

        if (casque != 0) fausses++;
        pireCasque = std::max(pireCasque, std::abs(casque));
        pireSimu = std::max(pireSimu, std::abs(simulateur));

        std::cout << t << " CASQUE " << casque << " SIMULATEUR " << simulateur << "\n";
    }

    std::cout << "FAUSSES " << fausses << "\n";
    std::cout << "PIRE CASQUE " << pireCasque << "\n";
    std::cout << "PIRE SIMULATEUR " << pireSimu << "\n";

    return 0;
}