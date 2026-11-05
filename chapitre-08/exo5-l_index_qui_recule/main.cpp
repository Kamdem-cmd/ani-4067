#include <iostream>
#include <vector>
#include <cstdint>

struct Result {
    uint64_t n;
    uint64_t r;
    bool refusee;
    uint64_t prochaine;
    uint64_t juste;
    int signe;
    uint32_t nonsigne;
};

int main() {
    // Optimisation des flux E/S
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    std::vector<Result> results;
    int refuseesCount = 0;
    int fauxSigneCount = 0;
    int fauxNonSigneCount = 0;

    for (int i = 0; i < N; ++i) {
        uint64_t n = 0, r = 0;
        if (!(std::cin >> n >> r)) break;

        Result res;
        res.n = n;
        res.r = r;

        if (n == 0 || n > 4) {
            res.refusee = true;
            refuseesCount++;
        } else {
            res.refusee = false;
            res.prochaine = r % n;
            res.juste = (res.prochaine + n - 1) % n;

            // Calcul signé en int (ex: -1 % n)
            int64_t proc_signed = static_cast<int64_t>(res.prochaine);
            res.signe = static_cast<int>((proc_signed - 1) % static_cast<int64_t>(n));

            // Calcul non signé en uint32_t (gestion de l'underflow)
            uint32_t proc_ns = static_cast<uint32_t>(res.prochaine);
            res.nonsigne = (proc_ns - 1) % static_cast<uint32_t>(n);

            if (static_cast<uint64_t>(res.signe) != res.juste) {
                fauxSigneCount++;
            }
            if (static_cast<uint64_t>(res.nonsigne) != res.juste) {
                fauxNonSigneCount++;
            }
        }
        results.push_back(res);
    }

    // Affichage ligne par ligne
    for (const auto& res : results) {
        if (res.refusee) {
            std::cout << res.n << " " << res.r << " REFUSEE\n";
        } else {
            std::cout << res.n << " " << res.r << " " << res.prochaine << " " << res.juste << " " << res.signe << " " << res.nonsigne << "\n";
        }
    }

    // Bilan final strict
    std::cout << "REFUSEES " << refuseesCount << "\n";
    std::cout << "FAUX SIGNE " << fauxSigneCount << "\n";
    std::cout << "FAUX NON SIGNE " << fauxNonSigneCount << "\n";

    return 0;
}