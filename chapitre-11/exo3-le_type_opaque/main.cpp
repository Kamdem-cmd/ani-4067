#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>

struct Fichier {
    std::string nom;
    std::vector<std::string> includes;
};

bool endsWithCpp(const std::string& s) {
    return s.size() >= 4 && s.substr(s.size() - 4) == ".cpp";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int E = 0;
    if (!(std::cin >> E)) return 0;

    std::set<std::string> externes;
    for (int i = 0; i < E; ++i) {
        std::string ext;
        std::cin >> ext;
        externes.insert(ext);
    }

    int F = 0;
    std::cin >> F;

    std::vector<Fichier> fichiers(F);
    std::map<std::string, std::vector<std::string>> adj;

    for (int i = 0; i < F; ++i) {
        int k = 0;
        std::cin >> fichiers[i].nom >> k;
        fichiers[i].includes.resize(k);
        for (int j = 0; j < k; ++j) {
            std::cin >> fichiers[i].includes[j];
        }
        adj[fichiers[i].nom] = fichiers[i].includes;
    }

    int touchees = 0;
    int unites = 0;

    for (const auto& f : fichiers) {
        if (!endsWithCpp(f.nom)) continue;
        unites++;

        std::deque<std::string> queue;
        std::set<std::string> visited;
        std::map<std::string, std::string> parent;

        queue.push_back(f.nom);
        visited.insert(f.nom);

        std::string foundExt;

        while (!queue.empty() && foundExt.empty()) {
            std::string curr = queue.front();
            queue.pop_front();

            auto it = adj.find(curr);
            if (it == adj.end()) continue;

            for (const auto& inc : it->second) {
                if (visited.count(inc)) continue;
                visited.insert(inc);
                parent[inc] = curr;

                if (externes.count(inc)) {
                    foundExt = inc;
                    break;
                }
                if (adj.count(inc)) {
                    queue.push_back(inc);
                }
            }
        }

        if (foundExt.empty()) {
            std::cout << f.nom << " NON\n";
        } else {
            touchees++;
            std::vector<std::string> path;
            std::string curr = foundExt;
            while (curr != f.nom) {
                path.push_back(curr);
                curr = parent[curr];
            }
            path.push_back(f.nom);

            std::cout << f.nom << " OUI ";
            for (size_t idx = path.size(); idx > 0; --idx) {
                std::cout << path[idx - 1];
                if (idx > 1) std::cout << ">";
            }
            std::cout << "\n";
        }
    }

    std::cout << "TOUCHEES " << touchees << " SUR " << unites << "\n";

    return 0;
}