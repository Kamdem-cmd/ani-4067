#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    long long muettes = 0;

    for (int i = 1; i <= N; ++i) {
        std::string fin, instant, pose, espace;
        int g = 0, d = 0;
        std::cin >> fin >> g >> d >> instant >> pose >> espace;

        std::vector<std::string> symptomes;
        if (fin == "JAMAIS") {
            symptomes.push_back("FIGE");
        } else if (fin == "SI_RENDU") {
            symptomes.push_back("NE_REPART_PAS");
        }

        if (g == 0) symptomes.push_back("OEIL_NOIR_G");
        if (d == 0) symptomes.push_back("OEIL_NOIR_D");
        if (instant == "N") symptomes.push_back("TREMBLE");
        if (pose == "F") symptomes.push_back("DE_TRAVERS");
        if (espace == "L") symptomes.push_back("DECALE");

        bool simuDit = (fin != "TOUJOURS") || (g == 0) || (d == 0);

        std::cout << i << " CASQUE";
        if (symptomes.empty()) {
            std::cout << " AUCUN";
        } else {
            for (const auto& s : symptomes) {
                std::cout << " " << s;
            }
        }
        std::cout << "\n";

        std::cout << i << " SIMULATEUR " << (simuDit ? "DIT" : "MUET") << "\n";

        if (!symptomes.empty() && !simuDit) {
            muettes++;
        }
    }

    std::cout << "MUETTES " << muettes << "\n";

    return 0;
}