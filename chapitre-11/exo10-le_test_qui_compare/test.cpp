#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long hz = 0, ipd = 0, N = 0;
    if (!(std::cin >> hz >> ipd >> N)) return 0;

    if (hz <= 1) hz = 72;
    long long periode = 1000000000LL / hz;

    long long x0 = 0, y0 = 0, z0 = 0, k0 = 0, dk = 0, vx = 0;
    std::cin >> x0 >> y0 >> z0 >> k0 >> dk >> vx;

    std::cout << "PERIODE " << periode << "\n";

    const long long droiteX[4] = {1, 0, -1, 0};
    const long long droiteZ[4] = {0, -1, 0, 1};
    long long halfIpd = ipd / 2;

    for (long long t = 1; t <= N; ++t) {
        long long step = t - 1;
        long long k = (k0 + step * dk) % 4;
        long long hx = x0 + step * vx;
        long long hy = y0;
        long long hzPos = z0;

        long long dx = droiteX[k] * halfIpd;
        long long dz = droiteZ[k] * halfIpd;

        long long gx = hx - dx;
        long long gy = hy;
        long long gz = hzPos - dz;

        long long rx = hx + dx;
        long long ry = hy;
        long long rz = hzPos + dz;

        long long instant = t * periode;

        std::cout << t
                  << " G " << gx << " " << gy << " " << gz
                  << " D " << rx << " " << ry << " " << rz
                  << " I " << instant << "\n";
    }

    return 0;
}