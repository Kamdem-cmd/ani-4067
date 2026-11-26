#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int K = 0;
    if (!(std::cin >> K)) return 0;

    std::string dummy;
    std::getline(std::cin, dummy);

    for (int i = 1; i <= K; ++i) {
        std::string regPath;
        std::getline(std::cin, regPath);

        int L = 0;
        std::cin >> L;
        std::getline(std::cin, dummy);

        if (regPath == "-") {
            std::cout << i << " REGISTRE AUCUN\n";
            continue;
        }

        std::cout << i << " MANIFESTE " << regPath << "\n";

        if (L == -1) {
            std::cout << i << " ECHEC ILLISIBLE\n";
            continue;
        }

        std::string contenu;
        for (int lineIdx = 0; lineIdx < L; ++lineIdx) {
            std::string line;
            std::getline(std::cin, line);
            contenu += line;
            contenu += '\n';
        }

        const std::string key = "\"library_path\"";
        size_t posKey = contenu.find(key);
        if (posKey == std::string::npos) {
            std::cout << i << " ECHEC CLE\n";
            continue;
        }

        size_t posColon = contenu.find(':', posKey + key.size());
        if (posColon == std::string::npos) {
            std::cout << i << " ECHEC DEUX_POINTS\n";
            continue;
        }

        size_t posQuote1 = contenu.find('"', posColon + 1);
        if (posQuote1 == std::string::npos) {
            std::cout << i << " ECHEC GUILLEMET\n";
            continue;
        }

        size_t posQuote2 = contenu.find('"', posQuote1 + 1);
        if (posQuote2 == std::string::npos) {
            std::cout << i << " ECHEC GUILLEMET\n";
            continue;
        }

        std::string valeur;
        for (size_t idx = posQuote1 + 1; idx < posQuote2; ++idx) {
            if (contenu[idx] == '\\' && idx + 1 < posQuote2 && contenu[idx + 1] == '\\') {
                continue; // saute le premier antislash d'une paire
            }
            valeur.push_back(contenu[idx]);
        }

        if (valeur.empty()) {
            std::cout << i << " ECHEC VIDE\n";
            continue;
        }

        std::cout << i << " VALEUR " << valeur << "\n";

        bool estAbsolu = false;
        if (valeur[0] == '\\' || valeur[0] == '/') {
            estAbsolu = true;
        } else if (valeur.size() >= 2 && valeur[1] == ':') {
            estAbsolu = true;
        }

        if (estAbsolu) {
            std::cout << i << " ABSOLU\n";
            std::cout << i << " CHARGE " << valeur << "\n";
        } else {
            std::cout << i << " RELATIF\n";
            size_t lastSlash = regPath.find_last_of("\\/");
            std::string dir = (lastSlash != std::string::npos) ? regPath.substr(0, lastSlash + 1) : "";
            std::cout << i << " CHARGE " << dir << valeur << "\n";
        }
    }

    return 0;
}