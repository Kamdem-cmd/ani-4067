#include <iostream>
#include <vector>
#include <cmath>

struct Pose {
    int x, y, k;
};

struct Objet {
    int id;
    Pose initialPose;
    Pose currentPose;
    bool saisi;
};

Pose rotatePose(Pose p, int k_rot) {
    int x = p.x, y = p.y;
    int k = (p.k + k_rot) % 4;
    for (int i = 0; i < k_rot; ++i) {
        int nx = -y;
        int ny = x;
        x = nx;
        y = ny;
    }
    return {x, y, k};
}

Pose composePoses(Pose A, Pose B) {
    Pose rotB = rotatePose(B, A.k);
    return {A.x + rotB.x, A.y + rotB.y, (A.k + B.k) % 4};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int M = 0, T = 0;
    if (!(std::cin >> M >> T)) return 0;

    std::vector<Objet> objets(M);
    for (int i = 0; i < M; ++i) {
        objets[i].id = i + 1;
        std::cin >> objets[i].initialPose.x >> objets[i].initialPose.y >> objets[i].initialPose.k;
        objets[i].currentPose = objets[i].initialPose;
        objets[i].saisi = false;
    }

    int saisies = 0;
    int ratees = 0;
    int objetSaisiId = -1;
    Pose relPoseOffset = {0, 0, 0};

    for (int t = 1; t <= T; ++t) {
        Pose paume;
        int b;
        std::cin >> paume.x >> paume.y >> paume.k >> b;

        if (b == 1) {
            if (objetSaisiId == -1) {
                // Trouver l'objet le plus proche
                int bestObjIdx = -1;
                double minDist = -1.0;
                for (int i = 0; i < M; ++i) {
                    double dist = std::hypot(objets[i].currentPose.x - paume.x, objets[i].currentPose.y - paume.y);
                    if (bestObjIdx == -1 || dist < minDist) {
                        minDist = dist;
                        bestObjIdx = i;
                    }
                }
                if (bestObjIdx != -1) {
                    objetSaisiId = objets[bestObjIdx].id;
                    objets[bestObjIdx].saisi = true;
                    saisies++;
                    std::cout << t << " TIENT " << objetSaisiId << " " << objets[bestObjIdx].currentPose.x << " " << objets[bestObjIdx].currentPose.y << " " << objets[bestObjIdx].currentPose.k << "\n";
                } else {
                    std::cout << t << " RATE\n";
                    ratees++;
                }
            } else {
                // Déjà un objet tenu, le faire suivre
                for (auto& obj : objets) {
                    if (obj.id == objetSaisiId) {
                        std::cout << t << " TIENT " << obj.id << " " << obj.currentPose.x << " " << obj.currentPose.y << " " << obj.currentPose.k << "\n";
                    }
                }
            }
        } else {
            if (objetSaisiId != -1) {
                objetSaisiId = -1;
            }
            std::cout << t << " LIBRE\n";
        }
    }

    std::cout << "SAISIES " << saisies << "\n";
    std::cout << "RATEES " << ratees << "\n";
    for (const auto& obj : objets) {
        std::cout << "OBJET " << obj.id << " " << obj.currentPose.x << " " << obj.currentPose.y << " " << obj.currentPose.k << "\n";
    }

    return 0;
}