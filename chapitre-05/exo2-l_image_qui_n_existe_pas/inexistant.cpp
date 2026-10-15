#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "VISIBLES 0\n";
        std::cout << "EN PANNE 0\n";
        return 0;
    }

    int visibles = 0;
    int en_panne = 0;

    for (int i = 0; i < n; ++i) {
        std::string nom;
        unsigned long long drapeaux = 0;
        long long sx = 0, sy = 0, sz = 0;
        long long distance = 0;
        long long lumieres = 0;
        long long ambiante = 0;
        long long proche = 0;

        std::cin >> nom >> drapeaux >> sx >> sy >> sz >> distance >> lumieres >> ambiante >> proche;

        long long face_avant = distance - (sz / 2);

        std::string verdict;

        if ((drapeaux & 2ULL) == 0) {
            verdict = "RENDER3D ETEINT";
        } else if (sx == 0 || sy == 0 || sz == 0) {
            verdict = "ECHELLE NULLE";
        } else if (face_avant <= 0) {
            verdict = "CAMERA DANS LE CUBE";
        } else if (face_avant < proche) {
            verdict = "COUPE PAR LE PLAN PROCHE";
        } else if (lumieres == 0 && ambiante == 0) {
            verdict = "PAS DE LUMIERE";
        } else {
            verdict = "VISIBLE";
        }

        if (verdict == "VISIBLE") {
            visibles++;
        } else {
            en_panne++;
        }

        std::cout << nom << " " << verdict << "\n";
    }

    std::cout << "VISIBLES " << visibles << "\n";
    std::cout << "EN PANNE " << en_panne << "\n";

    return 0;
}