#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    int bonnes = 0;
    int longues = 0;
    int muettes = 0;
    int breves = 0;
    uint64_t totalTemps = 0;

    for (int i = 1; i <= N; ++i) {
        int aRaw, dRaw, fRaw;
        std::cin >> aRaw >> dRaw >> fRaw;

        // Amplitude bridée entre 0 et 100
        int A = std::max(0, std::min(100, aRaw));

        // Durée
        std::string dStr;
        int DVal = 0;
        if (dRaw > 0) {
            DVal = dRaw;
            dStr = std::to_string(dRaw);
        } else {
            DVal = 0;
            dStr = "MIN";
        }

        // Fréquence
        std::string fStr;
        if (fRaw > 0) {
            fStr = std::to_string(fRaw);
        } else {
            fStr = "DEFAUT";
        }

        std::string verdict = "";
        if (A == 0) {
            verdict = "MUETTE";
            muettes++;
        } else if (DVal > 0 && DVal < 50) { // MIN vaut 0 donc < 50
            verdict = "BREVE";
            breves++;
        } else if (DVal <= 200) {
            verdict = "BONNE";
            bonnes++;
        } else {
            verdict = "LONGUE";
            longues++;
        }

        totalTemps += static_cast<uint64_t>(DVal);

        std::cout << i << " " << A << " " << dStr << " " << fStr << " " << verdict << "\n";
    }

    std::cout << "BONNES " << bonnes << "\n";
    std::cout << "LONGUES " << longues << "\n";
    std::cout << "MUETTES " << muettes << "\n";
    std::cout << "TEMPS " << totalTemps << "\n";

    return 0;
}