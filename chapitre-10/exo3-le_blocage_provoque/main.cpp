#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string P;
    int T = 0;
    if (!(std::cin >> P >> T)) return 0;

    std::string signes;
    if (T > 0) {
        std::cin >> signes;
    }

    bool trameOuverte = false;
    long long rendues = 0;
    long long perdues = 0;
    long long erreurs = 0;
    int premierRefus = -1;

    for (int t = 1; t <= T; ++t) {
        int signe = signes[t - 1] - '0';
        bool rendre = false;

        std::string wRes, bRes, rRes, eRes;

        // 1. WaitFrame
        if (trameOuverte) {
            wRes = "REFUSE";
            erreurs++;
            if (premierRefus == -1) premierRefus = t;
        } else {
            wRes = "OK";
            rendre = (signe == 1);
        }

        // 2. BeginFrame
        if (trameOuverte) {
            bRes = "REFUSE";
            erreurs++;
        } else {
            bRes = "OK";
            trameOuverte = true;
        }

        // 3. Rendu
        if (rendre) {
            rRes = "OUI";
            rendues++;
        } else {
            rRes = "NON";
            if (signe == 1) perdues++;
        }

        // 4. EndFrame
        bool callEnd = (P == "TOUJOURS") || (P == "SAUTE" && rendre);
        if (!callEnd) {
            eRes = "-";
        } else {
            if (!trameOuverte) {
                eRes = "REFUSE";
                erreurs++;
            } else {
                eRes = "OK";
                trameOuverte = false;
            }
        }

        std::cout << t << " " << wRes << " " << bRes << " " << rRes << " " << eRes << "\n";
    }

    std::cout << "RENDUES " << rendues << "\n";
    std::cout << "PERDUES " << perdues << "\n";
    std::cout << "ERREURS " << erreurs << "\n";
    if (premierRefus == -1) {
        std::cout << "PREMIER REFUS AUCUN\n";
    } else {
        std::cout << "PREMIER REFUS " << premierRefus << "\n";
    }

    return 0;
}