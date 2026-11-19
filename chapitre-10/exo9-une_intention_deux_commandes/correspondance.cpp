#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>

struct Action {
    std::string nom;
    std::string usage;
};

struct SimulationResult {
    std::vector<std::string> lignes;
};

SimulationResult simulerTable(const std::vector<Action>& actions, int tableIdx) {
    int U = 0;
    std::cin >> U;

    std::map<std::string, std::string> usageToCmd;
    for (int i = 0; i < U; ++i) {
        std::string usage, cmd;
        std::cin >> usage >> cmd;
        usageToCmd[usage] = cmd;
    }

    SimulationResult res;
    res.lignes.push_back("TABLE " + std::to_string(tableIdx));

    // Actions inactives (dont l'usage manque dans la table)
    for (const auto& action : actions) {
        if (usageToCmd.find(action.usage) == usageToCmd.end()) {
            res.lignes.push_back("INACTIVE " + action.nom);
        }
    }

    int T = 0;
    std::cin >> T;

    // Suivi des appuis précédents pour chaque action
    std::set<std::string> prevTenues;

    for (int t = 1; t <= T; ++t) {
        int k = 0;
        std::cin >> k;
        std::set<std::string> tenuesActuelles;
        for (int j = 0; j < k; ++j) {
            std::string c;
            std::cin >> c;
            tenuesActuelles.insert(c);
        }

        std::string ligneImage = std::to_string(t);
        bool anyAppuye = false;

        for (const auto& action : actions) {
            if (usageToCmd.find(action.usage) == usageToCmd.end()) {
                continue; // Action inactive
            }
            std::string cmd = usageToCmd[action.usage];
            bool estTenue = (tenuesActuelles.count(cmd) > 0);
            bool etaitTenue = (prevTenues.count(action.nom) > 0);

            if (estTenue && !etaitTenue) {
                ligneImage += " " + action.nom;
                anyAppuye = true;
            }
        }

        if (!anyAppuye) {
            ligneImage += " -";
        }

        res.lignes.push_back(ligneImage);

        // Mettre à jour l'état précédent
        prevTenues.clear();
        for (const auto& action : actions) {
            if (usageToCmd.find(action.usage) != usageToCmd.end()) {
                if (tenuesActuelles.count(usageToCmd[action.usage]) > 0) {
                    prevTenues.insert(action.nom);
                }
            }
        }
    }

    return res;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int A = 0;
    if (!(std::cin >> A)) return 0;

    std::vector<Action> actions(A);
    for (int i = 0; i < A; ++i) {
        std::cin >> actions[i].nom >> actions[i].usage;
    }

    auto res1 = simulerTable(actions, 1);
    auto res2 = simulerTable(actions, 2);

    for (const auto& l : res1.lignes) std::cout << l << "\n";
    for (const auto& l : res2.lignes) std::cout << l << "\n";

    if (res1.lignes == res2.lignes) {
        std::cout << "IDENTIQUES OUI\n";
    } else {
        std::cout << "IDENTIQUES NON\n";
    }

    return 0;
}