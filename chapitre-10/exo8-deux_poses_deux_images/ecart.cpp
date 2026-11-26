#include <iostream>
#include <cmath>
#include <cstdlib>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long N = 0, d = 0;
    if (!(std::cin >> N >> d)) return 0;

    long long sommePos = 0;
    long long sommeAngle = 0;

    for (long long t = 1; t <= N; ++t) {
        long long x1, z1, a1, x2, z2, a2;
        std::cin >> x1 >> z1 >> a1 >> x2 >> z2 >> a2;

        double dx = static_cast<double>(x2 - x1);
        double dz = static_cast<double>(z2 - z1);
        long long p = std::llround(std::sqrt(dx * dx + dz * dz));
        long long a = std::llabs(a2 - a1);

        sommePos += p;
        sommeAngle += a;

        std::cout << t << " POSITION " << p << " ANGLE " << a << "\n";
    }

    if (N == 0 || d == 0) {
        std::cout << "AUCUNE\n";
    } else {
        long long moyPos = sommePos / N;
        long long moyAngle = sommeAngle / N;
        long long vitesse = sommeAngle / (N * d);

        std::cout << "MOYENNE POSITION " << moyPos << "\n";
        std::cout << "MOYENNE ANGLE " << moyAngle << "\n";
        std::cout << "VITESSE " << vitesse << "\n";
    }

    return 0;
}