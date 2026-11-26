#include <iostream>
#include <string>
#include <vector>
#include <cctype>

bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool containsIdentifier(const std::string& text, const std::string& name) {
    if (name.empty()) return false;
    size_t pos = 0;
    while ((pos = text.find(name, pos)) != std::string::npos) {
        bool leftOk = (pos == 0) || !isWordChar(text[pos - 1]);
        size_t after = pos + name.size();
        bool rightOk = (after == text.size()) || !isWordChar(text[after]);
        if (leftOk && rightOk) {
            return true;
        }
        pos += 1;
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string tagC;
    int k = 0;
    if (!(std::cin >> tagC >> k)) return 0;

    std::vector<std::string> nomsC(k);
    for (int i = 0; i < k; ++i) {
        std::cin >> nomsC[i];
    }

    std::string tagD;
    int m = 0;
    std::cin >> tagD >> m;

    std::vector<std::string> nomsD(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> nomsD[i];
    }

    int N = 0;
    std::cin >> N;

    std::string dummy;
    std::getline(std::cin, dummy);

    long long decisions = 0;
    long long fuites = 0;
    long long definis = 0;

    for (int l = 1; l <= N; ++l) {
        std::string fullLine;
        std::getline(std::cin, fullLine);

        size_t spacePos = fullLine.find(' ');
        std::string zone = (spacePos == std::string::npos) ? fullLine : fullLine.substr(0, spacePos);
        std::string texte = (spacePos == std::string::npos) ? "" : fullLine.substr(spacePos + 1);

        // Ignorer les commentaires (espaces de début ôtés)
        size_t firstNonSpace = 0;
        while (firstNonSpace < texte.size() && std::isspace(static_cast<unsigned char>(texte[firstNonSpace]))) {
            firstNonSpace++;
        }
        if (firstNonSpace + 1 < texte.size() && texte[firstNonSpace] == '/' && texte[firstNonSpace + 1] == '/') {
            continue;
        }

        bool hasC = false;
        for (const auto& cName : nomsC) {
            if (containsIdentifier(texte, cName)) {
                hasC = true;
                break;
            }
        }

        bool hasD = false;
        for (const auto& dName : nomsD) {
            if (containsIdentifier(texte, dName)) {
                hasD = true;
                break;
            }
        }

        if (hasC) {
            if (zone == "INIT") {
                decisions++;
            } else if (zone == "BOUCLE" || zone == "SCENE") {
                std::cout << "FUITE " << l << " " << zone << "\n";
                fuites++;
            }
        }

        if (hasD && zone != "FABRIQUE") {
            std::cout << "DEFINI " << l << " " << zone << "\n";
            definis++;
        }
    }

    std::cout << "DECISIONS " << decisions << "\n";
    std::cout << "FUITES " << fuites << "\n";
    std::cout << "DEFINIS " << definis << "\n";
    std::cout << "ABSTRACTION " << ((fuites == 0 && definis == 0) ? "TENUE" : "FUIT") << "\n";

    return 0;
}