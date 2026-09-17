# Compte-rendu d'exercice — Non-commutativité de l'Ordre des Transformations

---

## 1. Démonstration Physique (Expérience Manipulée)

**Position initiale de référence :** L'objet (ex. une bouteille ou un téléphone) est posé à l'origine $(0, 0, 0)$, orienté vers le Nord (axe $+Z$).

| Séquence | Action 1 | Action 2 | Position Finale | Orientation Finale |
| :--- | :--- | :--- | :---: | :---: |
| **Séquence A**<br>*(Tourner puis Avancer)* | Rotation de $90^\circ$ vers la droite (Est). | Avancer de 2 mètres tout droit. | **$(2, 0, 0)$** | Est ($+X$) |
| **Séquence B**<br>*(Avancer puis Tourner)* | Avancer de 2 mètres tout droit (Nord). | Rotation de $90^\circ$ vers la droite (Est). | **$(0, 0, 2)$** | Est ($+X$) |

> **Constat visuel :** Bien que l'orientation finale soit identique dans les deux cas, la **position finale de l'objet est différente**. L'ordre des opérations modifie le point d'arrivée dans l'espace.

---

## 2. Formalisation Mathématique (Algèbre Matricielle)

En synthèse d'image 3D, appliquer une transformation équivaut à multiplier un vecteur position $v$ par une matrice de transformation $M$.

Soient :
* $T$ : Matrice de translation de $+2$ unités sur l'axe $Z$.
* $R$ : Matrice de rotation de $90^\circ$ autour de l'axe $Y$.

### Ordre A : $M_A = T \cdot R$ *(Rotation puis Translation)*
$$M_A = \begin{pmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 1 & 2 \\ 0 & 0 & 0 & 1 \end{pmatrix} \cdot \begin{pmatrix} 0 & 0 & 1 & 0 \\ 0 & 1 & 0 & 0 \\ -1 & 0 & 0 & 0 \\ 0 & 0 & 0 & 1 \end{pmatrix} = \begin{pmatrix} 0 & 0 & 1 & 0 \\ 0 & 1 & 0 & 0 \\ -1 & 0 & 0 & 2 \\ 0 & 0 & 0 & 1 \end{pmatrix}$$

Appliqué à l'origine $v = (0, 0, 0, 1)^T$ :
$$v'_A = M_A \cdot v = (0, 0, 2, 1)^T$$

### Ordre B : $M_B = R \cdot T$ *(Translation puis Rotation)*
$$M_B = \begin{pmatrix} 0 & 0 & 1 & 0 \\ 0 & 1 & 0 & 0 \\ -1 & 0 & 0 & 0 \\ 0 & 0 & 0 & 1 \end{pmatrix} \cdot \begin{pmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 1 & 2 \\ 0 & 0 & 0 & 1 \end{pmatrix} = \begin{pmatrix} 0 & 0 & 1 & 2 \\ 0 & 1 & 0 & 0 \\ -1 & 0 & 0 & 0 \\ 0 & 0 & 0 & 1 \end{pmatrix}$$

Appliqué à l'origine $v = (0, 0, 0, 1)^T$ :
$$v'_B = M_B \cdot v = (2, 0, 0, 1)^T$$

$$\mathbf{M_A \neq M_B \implies T \cdot R \neq R \cdot T}$$

---

## 3. Implémentation C++ & Sortie du Programme

### Code de vérification (C++ / Math Engine)

```cpp
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    glm::vec4 origin(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f));
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Séquence A : Tourner puis Avancer (M = T * R)
    glm::mat4 MA = T * R;
    glm::vec4 posA = MA * origin;

    // Séquence B : Avancer puis Tourner (M = R * T)
    glm::mat4 MB = R * T;
    glm::vec4 posB = MB * origin;

    std::cout << "--- RESULTATS D'EXECUTION PROGRAMME ---" << std::endl;
    std::cout << "Position A (T * R) : (" << posA.x << ", " << posA.y << ", " << posA.z << ")" << std::endl;
    std::cout << "Position B (R * T) : (" << posB.x << ", " << posB.y << ", " << posB.z << ")" << std::endl;

    return 0;
}