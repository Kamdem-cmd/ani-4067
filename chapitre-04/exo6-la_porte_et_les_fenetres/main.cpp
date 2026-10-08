#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long W = 0, H = 0, seuil = 0;
    if (!(std::cin >> W >> H >> seuil)) {
        std::cout << "OK 0\n";
        std::cout << "A REPRENDRE 0\n";
        return 0;
    }

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "OK 0\n";
        std::cout << "A REPRENDRE 0\n";
        return 0;
    }

    int total_ok = 0;
    int total_a_reprendre = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        long long u = 0, y = 0, l = 0, h = 0, e = 0, d = 0;
        std::cin >> nom >> u >> y >> l >> h >> e >> d;

        long long u_min = u - l / 2;
        long long u_max = u + l / 2;
        long long y_min = y - h / 2;
        long long y_max = y + h / 2;

        long long saillie = d + e / 2;
        long long arriere = d - e / 2;

        std::string verdict;

        if (u_min < -W / 2 || u_max > W / 2 || y_min < 0 || y_max > H) {
            verdict = "DEBORDE";
        } else if (saillie <= 0) {
            verdict = "INVISIBLE";
        } else if (saillie < seuil) {
            verdict = "CLIGNOTE";
        } else if (arriere > seuil) {
            verdict = "DECOLLE";
        } else {
            verdict = "OK";
        }

        if (verdict == "OK") {
            total_ok++;
        } else {
            total_a_reprendre++;
        }

        std::cout << nom << " " << saillie << " " << verdict << "\n";
    }

    std::cout << "OK " << total_ok << "\n";
    std::cout << "A REPRENDRE " << total_a_reprendre << "\n";

    return 0;
}