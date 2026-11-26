#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long hz = 0;
    if (!(std::cin >> hz)) return 0;

    if (hz <= 1) hz = 72;
    long long periode = 1000000000LL / hz;

    std::string modePose;
    std::cin >> modePose;

    long long lacet = 0, tangage = 0, px = 0, py = 1700, pz = 0;
    if (modePose == "FIXE") {
        std::cin >> lacet >> tangage >> px >> py >> pz;
    }

    int T = 0;
    std::cin >> T;

    std::vector<long long> horlogeA(T), horlogeB(T);
    for (int i = 0; i < T; ++i) {
        std::cin >> horlogeA[i];
    }
    for (int i = 0; i < T; ++i) {
        std::cin >> horlogeB[i];
    }

    std::cout << "PERIODE " << periode << "\n";
    std::cout << "POSE " << lacet << " " << tangage << " " << px << " " << py << " " << pz << "\n";

    int brutDiff = -1;
    int relatifDiff = -1;

    long long preditA0 = (T > 0) ? (horlogeA[0] + periode) : 0;
    long long preditB0 = (T > 0) ? (horlogeB[0] + periode) : 0;

    for (int i = 0; i < T; ++i) {
        long long t = i + 1;
        long long preditA = horlogeA[i] + periode;
        long long preditB = horlogeB[i] + periode;

        long long brut = preditB - preditA;
        long long relatif = (preditB - preditB0) - (preditA - preditA0);
        long long scripte = t * periode;

        if (brut != 0 && brutDiff == -1) brutDiff = static_cast<int>(t);
        if (relatif != 0 && relatifDiff == -1) relatifDiff = static_cast<int>(t);

        std::cout << t << " BRUT " << brut
                  << " RELATIF " << relatif
                  << " SCRIPTE " << scripte << "\n";
    }

    if (brutDiff == -1) {
        std::cout << "BRUT IDENTIQUE\n";
    } else {
        std::cout << "BRUT DIFFERENT " << brutDiff << "\n";
    }

    if (relatifDiff == -1) {
        std::cout << "RELATIF IDENTIQUE\n";
    } else {
        std::cout << "RELATIF DIFFERENT " << relatifDiff << "\n";
    }

    return 0;
}