#include <iostream>
#include <string>
#include <vector>
#include <set>

struct Vector2D {
    double x, z;
};

// Directions avant et droite selon le lacet k (en quarts de tour)
// k = 0 : avant ( 0, -1), droite ( 1,  0)
// k = 1 : avant (-1,  0), droite ( 0, -1)
// k = 2 : avant ( 0,  1), droite (-1,  0)
// k = 3 : avant ( 1,  0), droite ( 0,  1)
Vector2D getAvant(int k) {
    k = (k % 4 + 4) % 4;
    if (k == 0) return {0.0, -1.0};
    if (k == 1) return {-1.0, 0.0};
    if (k == 2) return {0.0, 1.0};
    return {1.0, 0.0}; // k == 3
}

Vector2D getDroite(int k) {
    k = (k % 4 + 4) % 4;
    if (k == 0) return {1.0, 0.0};
    if (k == 1) return {0.0, -1.0};
    if (k == 2) return {-1.0, 0.0};
    return {0.0, 1.0}; // k == 3
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    double v = 0.0;
    int T = 0;
    if (!(std::cin >> v >> T)) return 0;

    double posX = 0.0;
    double posZ = 0.0;
    int diagonalesCount = 0;

    for (int t = 1; t <= T; ++t) {
        int k = 0;
        int n = 0;
        std::cin >> k >> n;

        std::set<std::string> keys;
        for (int i = 0; i < n; ++i) {
            std::string c;
            std::cin >> c;
            keys.insert(c);
        }

        int dx = 0; // x : 1 si F_DROITE, -1 si F_GAUCHE
        int dy = 0; // y : 1 si F_HAUT, -1 si F_BAS

        if (keys.count("F_DROITE")) dx += 1;
        if (keys.count("F_GAUCHE")) dx -= 1;
        if (keys.count("F_HAUT")) dy += 1;
        if (keys.count("F_BAS")) dy -= 1;

        if (dx != 0 && dy != 0) {
            diagonalesCount++;
        }

        Vector2D avant = getAvant(k);
        Vector2D droite = getDroite(k);

        // Déplacement : v * (y * avant + x * droite)
        double moveX = v * (dy * avant.x + dx * droite.x);
        double moveZ = v * (dy * avant.z + dx * droite.z);

        posX += moveX;
        posZ += moveZ;

        // Affichage : t x,y X Z (avec X et Z entiers ou format standard)
        std::cout << t << " " << dx << "," << dy << " " << static_cast<long long>(posX) << " " << static_cast<long long>(posZ) << "\n";
    }

    std::cout << "DECALAGE " << static_cast<long long>(posX) << " " << static_cast<long long>(posZ) << "\n";
    std::cout << "DIAGONALES " << diagonalesCount << "\n";

    return 0;
}