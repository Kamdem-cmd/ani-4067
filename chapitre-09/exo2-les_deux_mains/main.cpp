#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

struct ActionDef {
    std::string nom;
    std::string usage;
    std::string main;
    std::string commande;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int A = 0, T = 0;
    if (!(std::cin >> A >> T)) return 0;

    std::vector<ActionDef> actions(A);
    int droiteCount = 0;
    int gaucheCount = 0;

    for (int i = 0; i < A; ++i) {
        std::cin >> actions[i].nom >> actions[i].usage;
        const auto& u = actions[i].usage;

        if (u == "SELECT") {
            actions[i].main = "DROITE";
            actions[i].commande = "SOURIS_G";
        } else if (u == "GRAB") {
            actions[i].main = "DROITE";
            actions[i].commande = "SOURIS_D";
        } else if (u == "MENU") {
            actions[i].main = "GAUCHE";
            actions[i].commande = "TAB";
        } else if (u == "MOVE") {
            actions[i].main = "GAUCHE";
            actions[i].commande = "FLECHES";
        } else if (u == "AIM_POSE" || u == "GRIP_POSE" || u == "HAPTIC") {
            actions[i].main = "DROITE";
            actions[i].commande = "AUCUNE";
        } else if (u == "SELECT_LEFT") {
            actions[i].main = "GAUCHE";
            actions[i].commande = "SOURIS_M";
        } else if (u == "GRAB_LEFT") {
            actions[i].main = "GAUCHE";
            actions[i].commande = "MAJ_G";
        } else if (u == "AIM_POSE_LEFT" || u == "GRIP_POSE_LEFT" || u == "HAPTIC_LEFT") {
            actions[i].main = "GAUCHE";
            actions[i].commande = "AUCUNE";
        } else if (u == "MOVE_RIGHT") {
            actions[i].main = "DROITE";
            actions[i].commande = "IJKL";
        }

        if (actions[i].main == "DROITE") droiteCount++;
        else gaucheCount++;

        std::cout << actions[i].nom << " " << actions[i].main << " " << actions[i].commande << "\n";
    }

    int teteCount = 0;

    for (int t = 1; t <= T; ++t) {
        int k = 0;
        std::cin >> k;
        std::set<std::string> keys;
        for (int j = 0; j < k; ++j) {
            std::string key;
            std::cin >> key;
            keys.insert(key);
        }

        // Vérification mouvement tête (Z, Q, S, D, W, A)
        if (keys.count("Z") || keys.count("Q") || keys.count("S") || 
            keys.count("D") || keys.count("W") || keys.count("A")) {
            teteCount++;
        }

        std::cout << t;

        for (int i = 0; i < A; ++i) {
            const auto& cmd = actions[i].commande;
            if (cmd == "AUCUNE") {
                std::cout << " -";
            } else if (cmd == "FLECHES") {
                int x = 0, y = 0;
                if (keys.count("F_DROITE")) x += 1;
                if (keys.count("F_GAUCHE")) x -= 1;
                if (keys.count("F_HAUT")) y += 1;
                if (keys.count("F_BAS")) y -= 1;
                std::cout << " " << x << "," << y;
            } else if (cmd == "IJKL") {
                int x = 0, y = 0;
                if (keys.count("L")) x += 1;
                if (keys.count("J")) x -= 1;
                if (keys.count("I")) y += 1;
                if (keys.count("K")) y -= 1;
                std::cout << " " << x << "," << y;
            } else {
                int val = keys.count(cmd) ? 1 : 0;
                std::cout << " " << val;
            }
        }
        std::cout << "\n";
    }

    std::cout << "DROITE " << droiteCount << "\n";
    std::cout << "GAUCHE " << gaucheCount << "\n";
    std::cout << "TETE " << teteCount << "\n";

    return 0;
}