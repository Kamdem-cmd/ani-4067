#include <iostream>
#include <string>

struct Chaine {
    int index = 0;
    bool empruntee = false;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int K = 0, N = 0;
    if (!(std::cin >> K >> N)) return 0;

    bool trameOuverte = false;
    Chaine gauche, droite;

    long long journal = 0;
    long long avertissements = 0;
    long long silencieux = 0;

    for (int i = 1; i <= N; ++i) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "wait") {
            if (trameOuverte) {
                std::cout << i << " wait REFUSE\n";
                journal++;
            } else {
                std::cout << i << " wait OK\n";
            }
        } else if (cmd == "begin") {
            if (trameOuverte) {
                std::cout << i << " begin REFUSE\n";
                journal++;
            } else {
                trameOuverte = true;
                std::cout << i << " begin OK\n";
            }
        } else if (cmd == "acquire") {
            std::string oeil;
            std::cin >> oeil;
            Chaine& c = (oeil == "G") ? gauche : droite;
            if (c.empruntee) {
                std::cout << i << " acquire REFUSE\n";
                silencieux++;
            } else {
                c.empruntee = true;
                std::cout << i << " acquire OK " << c.index << "\n";
            }
        } else if (cmd == "release") {
            std::string oeil;
            std::cin >> oeil;
            Chaine& c = (oeil == "G") ? gauche : droite;
            if (!c.empruntee) {
                std::cout << i << " release REFUSE\n";
                silencieux++;
            } else {
                c.empruntee = false;
                c.index = (c.index + 1) % K;
                std::cout << i << " release OK\n";
            }
        } else if (cmd == "end") {
            std::string arg1;
            std::cin >> arg1;
            bool hasProj = (arg1 != "-");
            int g = 0, d = 0;
            if (hasProj) {
                g = std::stoi(arg1);
                std::cin >> d;
            }

            if (!trameOuverte) {
                std::cout << i << " end REFUSE AUCUNE\n";
                journal++;
            } else if (hasProj && g == 1 && gauche.empruntee) {
                trameOuverte = false;
                std::cout << i << " end REFUSE ACQUISE G\n";
                journal++;
            } else if (hasProj && d == 1 && droite.empruntee) {
                trameOuverte = false;
                std::cout << i << " end REFUSE ACQUISE D\n";
                journal++;
            } else {
                trameOuverte = false;
                std::cout << i << " end OK";
                if (hasProj) {
                    if (g == 0) {
                        std::cout << " AVERT G";
                        avertissements++;
                    }
                    if (d == 0) {
                        std::cout << " AVERT D";
                        avertissements++;
                    }
                }
                std::cout << "\n";
            }
        }
    }

    std::cout << "JOURNAL " << journal << "\n";
    std::cout << "AVERTISSEMENTS " << avertissements << "\n";
    std::cout << "SILENCIEUX " << silencieux << "\n";

    return 0;
}