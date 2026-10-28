#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "DEPLACES 0\n";
        std::cout << "PIRE 0\n";
        return 0;
    }

    int deplaces = 0;
    long long pire = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long tx = 0, ty = 0, tz = 0;
        long long sx = 0, sy = 0, sz = 0;

        std::cin >> nom >> tx >> ty >> tz >> sx >> sy >> sz;

        // Mauvais ordre : échelle * translation / 1000
        long long nx = (sx * tx) / 1000;
        long long ny = (sy * ty) / 1000;
        long long nz = (sz * tz) / 1000;

        // Calcul du plus grand écart composante par composante
        long long diff_x = std::abs(tx - nx);
        long long diff_y = std::abs(ty - ny);
        long long diff_z = std::abs(tz - nz);

        long long ecart = std::max({diff_x, diff_y, diff_z});

        if (ecart > 0) {
            deplaces++;
        }

        if (ecart > pire) {
            pire = ecart;
        }

        std::cout << nom << " " << nx << " " << ny << " " << nz << " " << ecart << "\n";
    }

    std::cout << "DEPLACES " << deplaces << "\n";
    std::cout << "PIRE " << pire << "\n";

    return 0;
}