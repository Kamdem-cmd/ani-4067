#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "A CORRIGER 0\n";
        std::cout << "PIRE 0\n";
        return 0;
    }

    int a_corriger = 0;
    long long pire_ecart = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long e = 0, y = 0;
        std::cin >> nom >> e >> y;

        long long demi_hauteur = e / 2;
        long long y_bas = y - demi_hauteur;
        long long y_haut = y + demi_hauteur;
        long long y_pose = demi_hauteur;

        std::string verdict;

        if (y_haut <= 0) {
            verdict = "SOUS LE SOL";
        } else if (y_bas < 0) {
            verdict = "ENTERRE";
        } else if (y_bas == 0) {
            verdict = "POSE";
        } else {
            verdict = "FLOTTE";
        }

        if (verdict != "POSE") {
            a_corriger++;
        }

        long long ecart_abs = std::abs(y_bas);
        if (ecart_abs > pire_ecart) {
            pire_ecart = ecart_abs;
        }

        std::cout << nom << " " << y_bas << " " << y_haut << " " << verdict << " " << y_pose << "\n";
    }

    std::cout << "A CORRIGER " << a_corriger << "\n";
    std::cout << "PIRE " << pire_ecart << "\n";

    return 0;
}