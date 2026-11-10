#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Point3D {
    long long x, y, z;
};

// Rotation de (x, z) de k quarts de tour :
// 0 : (x, z)
// 1 : (z, -x)
// 2 : (-x, -z)
// 3 : (-z, x)
Point3D rotateXZ(long long x, long long z, int k) {
    k = (k % 4 + 4) % 4;
    if (k == 1) return {z, 0, -x};
    if (k == 2) return {-x, 0, -z};
    if (k == 3) return {-z, 0, x};
    return {x, 0, z}; // k == 0
}

Point3D transformPose(long long localX, long long localY, long long localZ, long long headX, long long headY, long long headZ, int k) {
    Point3D rot = rotateXZ(localX, localZ, k);
    return {
        rot.x + headX,
        localY + headY,
        rot.z + headZ
    };
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int H = 0, M = 0;
    if (!(std::cin >> H >> M)) return 0;

    struct Head {
        long long x, y, z;
        int k;
    };

    std::vector<Head> heads(H);
    for (int i = 0; i < H; ++i) {
        std::cin >> heads[i].x >> heads[i].y >> heads[i].z >> heads[i].k;
    }

    std::vector<Point3D> objets(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> objets[i].x >> objets[i].y >> objets[i].z;
    }

    // Coordonnées locales dans le repère de la tête
    // Paume droite : (180, -300, -250)
    // Visée droite : (180, -250, -400)
    // Paume gauche : (-180, -300, -250) (x change de signe AVANT la rotation)
    // Visée gauche : (-180, -250, -400)

    for (int h = 0; h < H; ++h) {
        long long hx = heads[h].x;
        long long hy = heads[h].y;
        long long hz = heads[h].z;
        int k = heads[h].k;

        Point3D paumeD = transformPose(180, -300, -250, hx, hy, hz, k);
        Point3D viseeD = transformPose(180, -250, -400, hx, hy, hz, k);
        Point3D paumeG = transformPose(-180, -300, -250, hx, hy, hz, k);
        Point3D viseeG = transformPose(-180, -250, -400, hx, hy, hz, k);

        std::cout << "TETE " << (h + 1) << "\n";
        std::cout << "PAUME D " << paumeD.x << " " << paumeD.y << " " << paumeD.z << "\n";
        std::cout << "VISEE D " << viseeD.x << " " << viseeD.y << " " << viseeD.z << "\n";
        std::cout << "PAUME G " << paumeG.x << " " << paumeG.y << " " << paumeG.z << "\n";
        std::cout << "VISEE G " << viseeG.x << " " << viseeG.y << " " << viseeG.z << "\n";

        // Recherche saisie paume droite (distance au carré <= 22500)
        int bestPaumeObj = -1;
        long long minPaumeDistSq = 22501;
        for (int i = 0; i < M; ++i) {
            long long dx = objets[i].x - paumeD.x;
            long long dy = objets[i].y - paumeD.y;
            long long dz = objets[i].z - paumeD.z;
            long long distSq = dx*dx + dy*dy + dz*dz;
            if (distSq <= 22500) {
                if (bestPaumeObj == -1 || distSq < minPaumeDistSq || (distSq == minPaumeDistSq && (i + 1) < bestPaumeObj)) {
                    minPaumeDistSq = distSq;
                    bestPaumeObj = i + 1;
                }
            }
        }

        if (bestPaumeObj != -1) {
            std::cout << "SAISIE PAUME " << bestPaumeObj << "\n";
        } else {
            std::cout << "SAISIE PAUME AUCUNE\n";
        }

        // Recherche saisie visée droite (distance au carré <= 22500)
        int bestViseeObj = -1;
        long long minViseeDistSq = 22501;
        for (int i = 0; i < M; ++i) {
            long long dx = objets[i].x - viseeD.x;
            long long dy = objets[i].y - viseeD.y;
            long long dz = objets[i].z - viseeD.z;
            long long distSq = dx*dx + dy*dy + dz*dz;
            if (distSq <= 22500) {
                if (bestViseeObj == -1 || distSq < minViseeDistSq || (distSq == minViseeDistSq && (i + 1) < bestViseeObj)) {
                    minViseeDistSq = distSq;
                    bestViseeObj = i + 1;
                }
            }
        }

        if (bestViseeObj != -1) {
            std::cout << "SAISIE VISEE " << bestViseeObj << "\n";
        } else {
            std::cout << "SAISIE VISEE AUCUNE\n";
        }
    }

    // Calcul de l'écart entre paume droite et visée droite (invariant par rapport à la tête)
    // Paume D locale : (180, -300, -250)
    // Visée D locale : (180, -250, -400)
    long long edx = 180 - 180;
    long long edy = -250 - (-300);
    long long edz = -400 - (-250);
    double ecart = std::sqrt(edx*edx + edy*edy + edz*edz);
    long long ecartArrondi = static_cast<long long>(std::round(ecart));

    std::cout << "ECART " << ecartArrondi << "\n";

    return 0;
}