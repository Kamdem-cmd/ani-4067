#include <iostream>
#include <vector>

void solveScenario(int n, int L, int T) {
    std::vector<int> peintes(T + 1, 0);
    int conflitsCount = 0;

    std::cout << "CIBLES " << n << " " << L;

    for (int u = 1; u <= T; ++u) {
        int img = (u - 1) % n;
        peintes[u] = img;
        std::cout << " " << img;

        // Vérification des conflits :
        // La trame u est en conflit si l'image qu'elle peint a été soumise par l'une des L trames d'avant (u-1 à u-L)
        bool enConflit = false;
        int debutWindow = (u - L > 1) ? (u - L) : 1;
        for (int prev = debutWindow; prev < u; ++prev) {
            if (peintes[prev] == img) {
                enConflit = true;
                break;
            }
        }
        if (enConflit) {
            conflitsCount++;
        }
    }
    std::cout << "\n";
    std::cout << "CONFLITS " << n << " " << L << " " << conflitsCount << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int K = 0;
    if (!(std::cin >> K)) return 0;

    for (int i = 0; i < K; ++i) {
        int n, L, T;
        std::cin >> n >> L >> T;
        solveScenario(n, L, T);
    }

    return 0;
}