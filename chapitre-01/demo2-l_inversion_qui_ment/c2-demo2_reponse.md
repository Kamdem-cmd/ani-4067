# Compte-rendu d'exercice — Inversion de Matrice Dégénérée & Impact VR

---

## 1. Implémentation C++ : Inversion Silencieuse avec Repli sur l'Identité

Voici la fonction d'inversion générale opérant sur une matrice $4 \times 4$. Lorsque la matrice est singulière ou dégénérée ($\det(M) \approx 0$), la fonction renvoie la matrice identité sans émettre le moindre message d'erreur ni assertion.

```cpp
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>

// Inversion générale avec repli silencieux sur l'identité
glm::mat4 safe_inverse(const glm::mat4& m, float epsilon = 1e-6f) {
    float det = glm::determinant(m);

    // Test de dégénérescence (déterminant nul ou quasi-nul)
    if (std::abs(det) < epsilon) {
        // Retourne la matrice identité sans aucun message de log/warning
        return glm::mat4(1.0f);
    }

    return glm::inverse(m);
}