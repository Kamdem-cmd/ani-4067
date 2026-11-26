#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int K = 0;
    if (!(std::cin >> K)) return 0;

    if (K == 0) {
        std::cout << "AUCUNE MACHINE\n";
        return 0;
    }

    for (int i = 1; i <= K; ++i) {
        std::string D, V, L, R, B, G;
        std::cin >> D >> V >> L >> R >> B >> G;

        int chargees = 0;
        int liberees = 0;
        bool libChargee = false;
        bool aPointEntree = false;
        std::string source;

        if (D == "SIMULATEUR") {
            std::cout << i << " BACKEND SIMULATEUR DEMANDE\n";
            if (G == "OUI") {
                std::cout << i << " NIVEAU 2 SIMULATEUR CARTE\n";
            } else {
                std::cout << i << " NIVEAU 3 SIMULATEUR LOGICIEL\n";
            }
            std::cout << i << " CHARGEES " << chargees << " LIBEREES " << liberees << "\n";
            continue;
        }

        // Règle 2 : Variable d'environnement
        if (V != "-") {
            if (V == "ABSENT") {
                std::cout << i << " VARIABLE IGNOREE\n";
            } else {
                std::cout << i << " VARIABLE CHARGEE\n";
                chargees++;
                libChargee = true;
                if (V == "BON") {
                    aPointEntree = true;
                    source = "VARIABLE";
                }
            }
        }

        // Règle 3 : Loader standard (seulement si aucune bibliothèque n'est chargée à ce moment)
        if (!libChargee) {
            if (L == "ABSENT") {
                std::cout << i << " LOADER ABSENT\n";
            } else {
                std::cout << i << " LOADER CHARGE\n";
                chargees++;
                libChargee = true;
                if (L == "BON") {
                    aPointEntree = true;
                    source = "LOADER";
                }
            }
        }

        // Règle 4 : Vérification du point d'entrée de la bibliothèque chargée (VARIABLE ou LOADER)
        if (libChargee && !aPointEntree) {
            std::cout << i << " SANS_ENTREE\n";
            libChargee = false;
            liberees++;
        }

        // Règle 5 : Découverte directe par le registre s'il n'y a toujours pas de point d'entrée
        if (!aPointEntree) {
            std::cout << i << " DIRECT " << R << "\n";
            if (R == "NEGO_KO" || R == "BON") {
                chargees++;
                libChargee = true;
                if (R == "BON") {
                    aPointEntree = true;
                    source = "DIRECT";
                }
            }
        }

        // Règle 6 : Résultat du backend OpenXR et liaison
        bool liaisonOk = false;
        if (!aPointEntree) {
            std::cout << i << " BACKEND INDISPONIBLE\n";
            if (libChargee) {
                libChargee = false;
                liberees++;
            }
        } else {
            std::cout << i << " BACKEND OPENXR " << source << "\n";
            if (B == "OUI") {
                liaisonOk = true;
            } else {
                std::cout << i << " LIAISON ECHOUEE\n";
                if (libChargee) {
                    libChargee = false;
                    liberees++;
                }
            }
        }

        // Règle 7 : Niveau atteint
        if (liaisonOk) {
            std::cout << i << " NIVEAU 1 CASQUE\n";
        } else if (G == "OUI") {
            std::cout << i << " NIVEAU 2 SIMULATEUR CARTE\n";
        } else {
            std::cout << i << " NIVEAU 3 SIMULATEUR LOGICIEL\n";
        }

        // Règle 8 : Bilan bibliothèques
        std::cout << i << " CHARGEES " << chargees << " LIBEREES " << liberees << "\n";
    }

    return 0;
}