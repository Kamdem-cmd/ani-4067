#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cctype>

bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool isVarBodyChar(char c) {
    return std::isupper(static_cast<unsigned char>(c)) ||
           std::isdigit(static_cast<unsigned char>(c)) ||
           c == '_';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long N = 0;
    if (!(std::cin >> N)) return 0;

    std::string dummy;
    std::getline(std::cin, dummy);

    std::vector<std::string> lines(N);
    for (long long i = 0; i < N; ++i) {
        std::getline(std::cin, lines[i]);
    }

    if (N == 0) {
        std::cout << "ENTETE 0\n";
        std::cout << "IGNOREES 0\n";
        std::cout << "VARIABLE AUCUNE\n";
        std::cout << "PART 0\n";
        return 0;
    }

    long long enteteCount = 0;
    std::vector<std::string> variables;
    std::set<std::string> vues;

    for (long long i = 0; i < N; ++i) {
        const std::string& line = lines[i];
        size_t pos = 0;
        while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) {
            pos++;
        }

        if (pos + 1 < line.size() && line[pos] == '/' && line[pos + 1] == '/') {
            enteteCount++;

            // Recherche des variables NK_...=
            size_t searchPos = pos + 2;
            while (searchPos + 3 < line.size()) {
                size_t found = line.find("NK_", searchPos);
                if (found == std::string::npos) break;

                bool beforeOk = (found == 0) || !isWordChar(line[found - 1]);
                if (beforeOk) {
                    size_t endVar = found + 3;
                    while (endVar < line.size() && isVarBodyChar(line[endVar])) {
                        endVar++;
                    }
                    if (endVar > found + 3 && endVar < line.size() && line[endVar] == '=') {
                        std::string varName = line.substr(found, endVar - found);
                        if (vues.find(varName) == vues.end()) {
                            vues.insert(varName);
                            variables.push_back(varName);
                        }
                        searchPos = endVar + 1;
                        continue;
                    }
                }
                searchPos = found + 1;
            }
        } else {
            break;
        }
    }

    long long ignorees = N - enteteCount;
    long long part = (100LL * enteteCount) / N;

    std::cout << "ENTETE " << enteteCount << "\n";
    std::cout << "IGNOREES " << ignorees << "\n";
    if (variables.empty()) {
        std::cout << "VARIABLE AUCUNE\n";
    } else {
        for (const auto& v : variables) {
            std::cout << "VARIABLE " << v << "\n";
        }
    }
    std::cout << "PART " << part << "\n";

    return 0;
}