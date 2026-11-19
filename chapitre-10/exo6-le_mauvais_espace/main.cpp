#include <iostream>
#include <string>

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

Pose composer(const Pose& ancre, const Pose& p) {
    long long rx, rz;
    tourner(p.x, p.z, ancre.k, rx, rz);
    return {
        ancre.x + rx,
        ancre.y + p.y,
        ancre.z + rz,
        (ancre.k + p.k) % 4
    };
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    Pose ancre;
    if (!(std::cin >> ancre.x >> ancre.y >> ancre.z >> ancre.k)) return 0;

    int N = 0;
    std::cin >> N;

    long long justes = 0;
    long long fausses = 0;

    for (int i = 1; i <= N; ++i) {
        char D, C;
        Pose p;
        std::cin >> D >> C >> p.x >> p.y >> p.z >> p.k;

        Pose vraie = (D == 'S') ? p : composer(ancre, p);
        Pose crue = (C == 'S') ? p : composer(ancre, p);

        long long dx = crue.x - vraie.x;
        long long dy = crue.y - vraie.y;
        long long dz = crue.z - vraie.z;
        int dk = (crue.k - vraie.k + 4) % 4;

        if (dx == 0 && dy == 0 && dz == 0 && dk == 0) {
            justes++;
        } else {
            fausses++;
        }

        std::cout << i << " CRU " << crue.x << " " << crue.y << " " << crue.z << " " << crue.k << "\n";
        std::cout << i << " ECART " << dx << " " << dy << " " << dz << " " << dk << "\n";
    }

    std::cout << "JUSTES " << justes << "\n";
    std::cout << "FAUSSES " << fausses << "\n";

    return 0;
}