#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T = 0;
    if (!(std::cin >> T)) return 0;

    std::string signs;
    if (T > 0) {
        std::cin >> signs;
    }

    int currentPrev = 0;
    bool avecTenu = false;
    bool sansTenu = false;
    bool sansTombe = false;

    int prisesAvec = 0;
    int prisesSans = 0;
    int chutesSans = 0;
    int coincesAvec = 0;
    int differences = 0;

    for (int t = 1; t <= T; ++t) {
        char sig = signs[t - 1];
        int active = (sig == 'x') ? 0 : 1;
        int rawCurrent = (sig == '1') ? 1 : 0;
        int current = (active == 0) ? 0 : rawCurrent;

        int changed = (current != currentPrev) ? 1 : 0;

        // Version AVEC
        if (active == 1) {
            if (current == 1 && changed == 1) {
                if (!avecTenu) prisesAvec++;
                avecTenu = true;
            } else if (current == 0 && changed == 1) {
                avecTenu = false;
            }
        }

        // Version SANS
        if (!sansTombe) {
            if (current == 1 && changed == 1) {
                if (!sansTenu) prisesSans++;
                sansTenu = true;
            } else if (current == 0 && changed == 1) {
                if (active == 0) {
                    sansTombe = true;
                    sansTenu = false;
                    chutesSans++;
                } else {
                    sansTenu = false;
                }
            }
        }

        std::string resAvec = avecTenu ? "TENU" : "LIBRE";
        std::string resSans = sansTenu ? "TENU" : "LIBRE";

        if (resAvec != resSans) {
            differences++;
        }

        if (active == 0 && avecTenu) {
            coincesAvec++;
        }

        std::cout << t << " " << resAvec << " " << resSans << "\n";
        currentPrev = current;
    }

    std::cout << "PRISES AVEC " << prisesAvec << "\n";
    std::cout << "PRISES SANS " << prisesSans << "\n";
    std::cout << "CHUTES SANS " << chutesSans << "\n";
    std::cout << "COINCES AVEC " << coincesAvec << "\n";
    std::cout << "DIFFERENCES " << differences << "\n";

    return 0;
}