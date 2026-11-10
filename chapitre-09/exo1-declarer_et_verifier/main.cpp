#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

struct Action {
    std::string nom;
    std::string type;
    int usageNum;
};

int getUsageNum(const std::string& usage) {
    if (usage == "SELECT") return 0;
    if (usage == "GRAB") return 1;
    if (usage == "MENU") return 2;
    if (usage == "MOVE") return 3;
    if (usage == "AIM_POSE") return 4;
    if (usage == "GRIP_POSE") return 5;
    if (usage == "HAPTIC") return 6;
    if (usage == "SELECT_LEFT") return 7;
    if (usage == "GRAB_LEFT") return 8;
    if (usage == "AIM_POSE_LEFT") return 9;
    if (usage == "GRIP_POSE_LEFT") return 10;
    if (usage == "HAPTIC_LEFT") return 11;
    if (usage == "MOVE_RIGHT") return 12;
    return 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N = 0;
    if (!(std::cin >> N)) return 0;

    std::vector<Action> actions;
    std::unordered_set<std::string> nomsVus;

    bool estAttache = false;
    int kAttachees = 0;
    int nomsEnDouble = 0;

    for (int i = 0; i < N; ++i) {
        std::string cmd;
        if (!(std::cin >> cmd)) break;

        if (cmd == "cree") {
            std::string nom, type, usageStr;
            std::cin >> nom >> type >> usageStr;

            if (nomsVus.count(nom) > 0) {
                nomsEnDouble++;
            } else {
                nomsVus.insert(nom);
            }

            int uNum = getUsageNum(usageStr);
            actions.push_back({nom, type, uNum});
            int h = static_cast<int>(actions.size());

            std::cout << "ACTION " << h << " " << nom << " " << type << " " << uNum << "\n";
        }
        else if (cmd == "attache") {
            if (estAttache) {
                std::cout << "ATTACHE REFUSE\n";
            } else {
                estAttache = true;
                kAttachees = static_cast<int>(actions.size());
                std::cout << "ATTACHE " << kAttachees << "\n";
            }
        }
        else if (cmd == "lit") {
            uint64_t h;
            std::cin >> h;

            if (h == 0 || h > actions.size()) {
                std::cout << "LIT " << h << " INVALIDE\n";
            } else if (!estAttache || h > static_cast<uint64_t>(kAttachees)) {
                std::cout << "LIT " << h << " INCONNUE\n";
            } else {
                std::cout << "LIT " << h << " " << actions[h - 1].nom << "\n";
            }
        }
    }

    // Bilan final strict
    std::cout << "ACTIONS " << actions.size() << "\n";
    std::cout << "ATTACHEES " << kAttachees << "\n";
    std::cout << "NOMS EN DOUBLE " << nomsEnDouble << "\n";

    return 0;
}