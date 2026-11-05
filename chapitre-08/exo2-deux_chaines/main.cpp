#include <iostream>
#include <string>
#include <vector>

struct ChaineInfo {
    std::string nom;
    int largeur;
    int hauteur;
    int images;
    int oeil; // 0 = gauche, 1 = droit
    bool acceptee;
    std::string motifRefus; // "TAILLE" ou "IMAGES"
};

int main() {
    // Optimisation des flux E/S
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N;
    if (!(std::cin >> N)) return 0;

    std::vector<ChaineInfo> chaines(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> chaines[i].largeur >> chaines[i].hauteur >> chaines[i].images >> chaines[i].oeil >> chaines[i].nom;
    }

    int accepteesCount = 0;
    int emplacementsCount = 0;
    int occupesCount = 0;
    int gaucheCount = 0;
    int droitCount = 0;
    int sansNomCount = 0;

    std::vector<std::string> lignesAffichage;

    for (int i = 0; i < N; ++i) {
        const auto& c = chaines[i];
        std::string nomAffichage = (c.nom == "-") ? "SANS-NOM" : c.nom;

        // Règle 2 : Si largeur ou hauteur vaut 0, refusé pour TAILLE (contrôle en premier)
        if (c.largeur == 0 || c.hauteur == 0) {
            lignesAffichage.push_back("REFUSEE " + nomAffichage + " TAILLE");
        }
        // Règle 3 : Sinon, si images vaut 0 ou plus de 4, refusé pour IMAGES
        else if (c.images == 0 || c.images > 4) {
            lignesAffichage.push_back("REFUSEE " + nomAffichage + " IMAGES");
        }
        // Règle 4 : Acceptée
        else {
            accepteesCount++;
            emplacementsCount += 4; // Chacune réserve un tableau fixe de 4 emplacements
            occupesCount += c.images;

            if (c.oeil == 0) {
                gaucheCount++;
            } else {
                droitCount++;
            }

            if (c.nom == "-") {
                sansNomCount++;
            }

            std::string oeilStr = (c.oeil == 0) ? "GAUCHE" : "DROIT";
            lignesAffichage.push_back("CHAINE " + nomAffichage + " " + std::to_string(c.largeur) + " " + std::to_string(c.hauteur) + " " + std::to_string(c.images) + " " + oeilStr);
        }
    }

    // Affichage des lignes de résultats individuels
    for (const auto& ligne : lignesAffichage) {
        std::cout << ligne << "\n";
    }

    // Affichage du bilan final (6 lignes strictes)
    std::cout << "ACCEPTEES " << accepteesCount << "\n";
    std::cout << "EMPLACEMENTS " << emplacementsCount << "\n";
    std::cout << "OCCUPES " << occupesCount << "\n";
    std::cout << "GAUCHE " << gaucheCount << "\n";
    std::cout << "DROIT " << droitCount << "\n";
    std::cout << "SANS NOM " << sansNomCount << "\n";

    return 0;
}