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
    int appuis = 0;
    int relaches = 0;
    int fauxRelaches = 0;
    int tenues = 0;

    for (int t = 1; t <= T; ++t) {
        char sig = signs[t - 1];
        int active = (sig == 'x') ? 0 : 1;
        int rawCurrent = (sig == '1') ? 1 : 0;
        int current = (active == 0) ? 0 : rawCurrent;

        int changed = (current != currentPrev) ? 1 : 0;

        if (current == 1 && changed == 1) {
            appuis++;
        }
        if (current == 0 && changed == 1) {
            relaches++;
            if (active == 0) {
                fauxRelaches++;
            }
        }
        if (current == 1) {
            tenues++;
        }

        std::cout << t << " " << current << " " << changed << " " << active << "\n";
        currentPrev = current;
    }

    std::cout << "APPUIS " << appuis << "\n";
    std::cout << "RELACHES " << relaches << "\n";
    std::cout << "FAUX RELACHES " << fauxRelaches << "\n";
    std::cout << "TENUES " << tenues << "\n";

    return 0;
}