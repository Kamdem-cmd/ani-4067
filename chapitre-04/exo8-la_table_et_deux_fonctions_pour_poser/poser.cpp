#include <iostream>
#include <string>
#include <vector>

struct Vector3 {
    long long x;
    long long y;
    long long z;
};

// Calcule le centre d'un objet posé au sol
Vector3 PoserAuSol(long long x, long long z, long long sy) {
    return {x, sy / 2, z};
}

// Calcule le centre d'un objet posé sur une table à la hauteur de dessus H
Vector3 PoserSurTable(long long x, long long z, long long sy, long long H) {
    return {x, H + sy / 2, z};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long L = 0, P = 0, H = 0, ep = 0, pied = 0, tx = 0, tz = 0;
    if (!(std::cin >> L >> P >> H >> ep >> pied >> tx >> tz)) {
        return 0;
    }

    // 1. Calcul et affichage du Plateau
    // Le haut du plateau est à H, donc son centre y vaut H - ep / 2
    long long plateau_y = H - ep / 2;
    std::cout << "PLATEAU " << tx << " " << plateau_y << " " << tz << "\n";

    // 2. Calcul et affichage des quatre pieds
    // Hauteur d'un pied : H - ep, posé au sol -> centre y = (H - ep) / 2
    long long h_pied = H - ep;
    long long pied_y = h_pied / 2;

    long long dx = L / 2 - pied;
    long long dz = P / 2 - pied;

    // Ordre imposé : moins-moins, plus-moins, moins-plus, plus-plus
    std::cout << "PIED " << (tx - dx) << " " << pied_y << " " << (tz - dz) << "\n";
    std::cout << "PIED " << (tx + dx) << " " << pied_y << " " << (tz - dz) << "\n";
    std::cout << "PIED " << (tx - dx) << " " << pied_y << " " << (tz + dz) << "\n";
    std::cout << "PIED " << (tx + dx) << " " << pied_y << " " << (tz + dz) << "\n";

    // 3. Lecture et positionnement des objets
    int N = 0;
    if (std::cin >> N) {
        for (int i = 0; i < N; ++i) {
            std::string nom, ou;
            long long sx = 0, sy = 0, sz = 0, x = 0, z = 0;
            std::cin >> nom >> sx >> sy >> sz >> x >> z >> ou;

            Vector3 pos;
            if (ou == "TABLE") {
                pos = PoserSurTable(x, z, sy, H);
            } else {
                pos = PoserAuSol(x, z, sy);
            }

            std::cout << nom << " " << pos.x << " " << pos.y << " " << pos.z << "\n";
        }
    }

    return 0;
}