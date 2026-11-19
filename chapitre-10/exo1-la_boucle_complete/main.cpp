#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T = 0;
    if (!(std::cin >> T)) return 0;

    int justes = 0;
    int fautives = 0;

    for (int t = 1; t <= T; ++t) {
        int n = 0;
        std::cin >> n;
        std::vector<std::string> appels(n);
        for (int i = 0; i < n; ++i) {
            std::cin >> appels[i];
        }

        bool hasWait = false;
        bool hasBegin = false;
        bool hasEnd = false;
        bool hasVues = false;
        bool prisG = false;
        bool prisD = false;

        int fautePos = -1;
        std::string fauteCode;

        for (int i = 0; i < n; ++i) {
            const std::string& cmd = appels[i];
            int pos = i + 1;

            if (hasEnd) {
                fautePos = pos;
                fauteCode = "APRES_END";
                break;
            }

            if (cmd == "WAIT") {
                if (hasBegin) {
                    fautePos = pos;
                    fauteCode = "WAIT_APRES_BEGIN";
                    break;
                }
                hasWait = true;
            } else if (cmd == "BEGIN") {
                if (!hasWait) {
                    fautePos = pos;
                    fauteCode = "BEGIN_SANS_WAIT";
                    break;
                }
                if (hasBegin) {
                    fautePos = pos;
                    fauteCode = "DOUBLE_BEGIN";
                    break;
                }
                hasBegin = true;
            } else if (cmd == "VUES") {
                if (!hasWait) {
                    fautePos = pos;
                    fauteCode = "VUES_SANS_WAIT";
                    break;
                }
                hasVues = true;
            } else if (cmd == "PREND_G") {
                if (!hasBegin) {
                    fautePos = pos;
                    fauteCode = "PREND_SANS_BEGIN";
                    break;
                }
                if (!hasVues) {
                    fautePos = pos;
                    fauteCode = "PREND_SANS_VUES";
                    break;
                }
                if (prisG) {
                    fautePos = pos;
                    fauteCode = "DOUBLE_PREND";
                    break;
                }
                prisG = true;
            } else if (cmd == "PREND_D") {
                if (!hasBegin) {
                    fautePos = pos;
                    fauteCode = "PREND_SANS_BEGIN";
                    break;
                }
                if (!hasVues) {
                    fautePos = pos;
                    fauteCode = "PREND_SANS_VUES";
                    break;
                }
                if (prisD) {
                    fautePos = pos;
                    fauteCode = "DOUBLE_PREND";
                    break;
                }
                prisD = true;
            } else if (cmd == "LACHE_G") {
                if (!prisG) {
                    fautePos = pos;
                    fauteCode = "LACHE_SANS_PREND";
                    break;
                }
                prisG = false;
            } else if (cmd == "LACHE_D") {
                if (!prisD) {
                    fautePos = pos;
                    fauteCode = "LACHE_SANS_PREND";
                    break;
                }
                prisD = false;
            } else if (cmd == "END") {
                if (!hasBegin) {
                    fautePos = pos;
                    fauteCode = "END_SANS_BEGIN";
                    break;
                }
                if (prisG || prisD) {
                    fautePos = pos;
                    fauteCode = "END_IMAGE_PRISE";
                    break;
                }
                hasEnd = true;
            }
        }

        if (fautePos == -1 && !hasEnd) {
            fautePos = n + 1;
            fauteCode = "SANS_END";
        }

        if (fautePos == -1) {
            std::cout << t << " OK\n";
            justes++;
        } else {
            std::cout << t << " FAUTE " << fautePos << " " << fauteCode << "\n";
            fautives++;
        }
    }

    std::cout << "JUSTES " << justes << "\n";
    std::cout << "FAUTIVES " << fautives << "\n";

    return 0;
}