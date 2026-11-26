#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct Pose {
    long long x, y, z;
    int k;
};

void tourner(long long x, long long z, int k, long long& outX, long long& outZ) {
    k = ((k % 4) + 4) % 4;
    if (k == 0) { outX = x; outZ = z; }
    else if (k == 1) { outX = z; outZ = -x; }
    else if (k == 2) { outX = -x; outZ = -z; }
    else { outX = -z; outZ = x; }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    Pose ancre;
    if (!(std::cin >> ancre.x >> ancre.y >> ancre.z >> ancre.k)) return 0;

    long long ex, ey, ez;
    std::cin >> ex >> ey >> ez;

    int N = 0;
    std::cin >> N;

    for (int i = 1; i <= N; ++i) {
        char E;
        long long px, py, pz, w, h;
        int pk;
        std::cin >> E >> px >> py >> pz >> pk >> w >> h;

        bool isStage = (E == 'S');
        std::string espaceLabel = isStage ? "STAGE" : "LOCAL";

        Pose centre;
        if (isStage) {
            centre = {px, py, pz, ((pk % 4) + 4) % 4};
        } else {
            long long rx, rz;
            tourner(px, pz, ancre.k, rx, rz);
            centre = {
                ancre.x + rx,
                ancre.y + py,
                ancre.z + rz,
                (ancre.k + pk) % 4
            };
        }

        std::cout << "PANNEAU " << i << " " << espaceLabel << "\n";

        const long long localCorners[4][2] = {
            {-w / 2, -h / 2},
            { w / 2, -h / 2},
            { w / 2,  h / 2},
            {-w / 2,  h / 2}
        };

        for (int c = 0; c < 4; ++c) {
            long long cx = localCorners[c][0];
            long long cy = localCorners[c][1];
            long long rcx, rcz;
            tourner(cx, 0, centre.k, rcx, rcz);
            std::cout << (centre.x + rcx) << " " << (centre.y + cy) << " " << (centre.z + rcz) << "\n";
        }

        double dx = static_cast<double>(centre.x - ex);
        double dy = static_cast<double>(centre.y - ey);
        double dz = static_cast<double>(centre.z - ez);
        long long dist = std::llround(std::sqrt(dx * dx + dy * dy + dz * dz));

        std::cout << "DISTANCE " << dist << "\n";
    }

    std::cout << "PANNEAUX " << N << "\n";

    return 0;
}