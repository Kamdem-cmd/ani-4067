#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

struct FrameRecord {
    int frame;
    int paintedFrame;
    double pos;
    double saut;
    bool hasSaut;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0, T = 0;
    double x0 = 0.0, v = 0.0;
    if (!(std::cin >> n >> T >> x0 >> v)) return 0;

    std::string modes;
    std::cin >> modes;

    // Chaque emplacement garde le numéro de la dernière trame peinte dedans (0 au départ)
    std::vector<int> chainSlots(n, 0);
    int prochaine = 0;
    int estEmprunteeIdx = -1;

    std::vector<FrameRecord> history;
    double prevPos = 0.0;
    bool prevShown = false;

    for (int t = 1; t <= T; ++t) {
        // 4. À la trame t : on emprunte la prochaine image, et l'on y peint la trame t
        int empruntee = prochaine;
        estEmprunteeIdx = empruntee;
        chainSlots[empruntee] = t; // Peint la trame t dedans

        double currentCamPos = x0 + v * t;

        // Détermination de l'index soumis selon le mode à la trame t
        char mode = modes[t - 1];
        int indexSoumis = -1;

        if (mode == 'A') {
            // Mode A : index emprunté
            indexSoumis = empruntee;
        } else if (mode == 'P') {
            // Mode P : lu avant la libération
            indexSoumis = (prochaine + n - 1) % n;
        } else if (mode == 'D') {
            // Mode D : lu après, avec la même formule (sur l'état courant de l'index au release)
            indexSoumis = (prochaine + n - 1) % n;
        }

        // 6. Puis on rend l'image : la prochaine devient (prochaine + 1) % n
        if (mode == 'D') {
            // En mode D, l'index soumis est lu après release ou avec la formule de base
            // (La consigne précise : "En mode D, l'index soumis est lu maintenant, avec la même formule")
            indexSoumis = (prochaine + n - 1) % n; // ou équivalent selon la logique de bascule
        }

        prochaine = (prochaine + 1) % n;
        estEmprunteeIdx = -1;

        // 7. La trame montrée est f, le numéro gardé par l'emplacement soumis
        int f = chainSlots[indexSoumis];

        if (f == 0) {
            std::cout << t << " NOIR -\n";
            prevShown = false;
        } else {
            double shownPos = x0 + v * f;
            double sautVal = 0.0;
            bool hasSaut = false;

            if (prevShown && t > 1) {
                sautVal = shownPos - prevPos;
                hasSaut = true;
                std::cout << t << " " << shownPos << " " << sautVal << "\n";
            } else {
                std::cout << t << " " << shownPos << " -\n";
            }

            prevPos = shownPos;
            prevShown = true;
            if (hasSaut) {
                history.push_back({t, f, shownPos, sautVal, true});
            }
        }
    }

    // Bilan final
    if (history.empty()) {
        std::cout << "SAUTS AUCUN\n";
    } else {
        double minSaut = history[0].saut;
        double maxSaut = history[0].saut;
        for (const auto& h : history) {
            if (h.saut < minSaut) minSaut = h.saut;
            if (h.saut > maxSaut) maxSaut = h.saut;
        }

        // Affichage propre des valeurs numériques
        std::cout << "SAUT MIN " << minSaut << "\n";
        std::cout << "SAUT MAX " << maxSaut << "\n";

        if (minSaut != maxSaut) {
            std::cout << "TREMBLE OUI\n";
        } else {
            std::cout << "TREMBLE NON\n";
        }
    }

    return 0;
}