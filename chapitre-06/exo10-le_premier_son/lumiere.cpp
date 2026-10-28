#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

struct Vector3 {
    double x, y, z;
};

// Calcule la norme (longueur) d'un vecteur
double Norme(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Produit scalaire de deux vecteurs
double ProduitScalaire(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long ambiante = 0;
    if (!(std::cin >> ambiante)) {
        return 0;
    }

    int n = 0;
    if (!(std::cin >> n)) {
        return 0;
    }

    // Définition des 5 faces et de leurs normales intérieures respectives
    const std::vector<std::pair<std::string, Vector3>> faces = {
        {"SOL",    { 0.0,  1.0,  0.0}},
        {"FOND",   { 0.0,  0.0,  1.0}},
        {"ENTREE", { 0.0,  0.0, -1.0}},
        {"GAUCHE", { 1.0,  0.0,  0.0}},
        {"DROIT",  {-1.0,  0.0,  0.0}}
    };

    for (int i = 0; i < n; ++i) {
        std::string nom;
        double dx = 0.0, dy = 0.0, dz = 0.0, I = 0.0;
        std::cin >> nom >> dx >> dy >> dz >> I;

        Vector3 dir_soleil = {dx, dy, dz};
        double len_dir = Norme(dir_soleil);

        long long max_lum = -1;
        long long min_lum = 1000000;

        for (const auto& face : faces) {
            const std::string& nom_face = face.first;
            const Vector3& normale = face.second;

            // c = - (normale . dir_soleil) / ||dir_soleil||
            double dot = ProduitScalaire(normale, dir_soleil);
            double c = -dot / len_dir;

            if (c < 0.0) {
                c = 0.0;
            }

            double lum_exact = static_cast<double>(ambiante) + I * c;
            long long lum_arrondi = static_cast<long long>(std::round(lum_exact));

            if (lum_arrondi > max_lum) max_lum = lum_arrondi;
            if (lum_arrondi < min_lum) min_lum = lum_arrondi;

            std::cout << nom << " " << nom_face << " " << lum_arrondi << "\n";
        }

        long long contraste = max_lum - min_lum;
        std::cout << nom << " CONTRASTE " << contraste << "\n";
    }

    return 0;
}