# Compte-rendu d'exercice — Interpolation de Quaternions & Forçage du Chemin Court (*Shortest Path*)

---

## 1. Mise en Évidence du Problème : Rotation Longue et Pic de Vitesse Angulaire

En algèbre des quaternions, $q$ et $-q$ représentent exactement la même orientation dans l'espace 3D (double recouvrement de $SO(3)$ par $S^3$). Cependant, lors d'une interpolation sphérique (**SLERP**), choisir le mauvais signe contraint le chemin géodésique à parcourir le grand arc de la hypersphère ($360^\circ - \theta$) au lieu du chemin le plus court ($\theta$).

### Démonstration avec un Delta Équivalent à $2^\circ$

Pour une rotation réelle de seulement $2^\circ$ sur un pas de temps de $\Delta t = 11{,}11 \text{ ms}$ (budget 90 Hz) :
* **Orientation initiale $q_1$ :** Identité $(w=1, x=0, y=0, z=0)$.
* **Orientation cible $q_2$ :** $2^\circ$ autour de l'axe Y, sous sa forme antipodale négative $-q_{2^\circ}$ ($q_1 \cdot q_2 < 0$).

#### Code Naïf (Sans forçage du chemin court)

```cpp
glm::quat slerp_naive(glm::quat q1, glm::quat q2, float t) {
    float dot = glm::dot(q1, q2);
    float theta = std::acos(dot);
    
    // Calcule l'interpolation sur le grand arc (presque 360°)
    return (std::sin((1.0f - t) * theta) * q1 + std::sin(t * theta) * q2) / std::sin(theta);
}
```

````
Mesure de la Vitesse Angulaire ($\omega$) sur la TrajectoireMétriqueValeur sans Forçage du Chemin CourtImpact PhysiqueAngle parcouru ($\Delta \theta$)$358^\circ$ (au lieu de $2^\circ$)Rotation sur le grand arc de la sphère.Durée du step ($\Delta t$)$11{,}11 \text{ ms}$1 frame VR à 90 Hz.Vitesse Angulaire ($\omega$)$32\,223^\circ/\text{s}$Pic aberrant : l'objet fait un tour complet sur lui-même en 1 frame.2. Correction : Les Trois Lignes de CodePour garantir la trajectoire géodésique minimale, il suffit de vérifier le signe du produit scalaire ($q_1 \cdot q_2$) et d'inverser les signes de $q_2$ si le produit est négatif :C++// --- LES 3 LIGNES DE CORRECTION ---
if (dot < 0.0f) {
    q2 = -q2;
    dot = -dot;
}
// -----------------------------------
Intégration dans la Fonction SLERP RHI (NkIDevice)C++glm::quat slerp_corrected(glm::quat q1, glm::quat q2, float t) {
    float dot = glm::dot(q1, q2);

    // 1. Forçage du chemin court (3 lignes)
    if (dot < 0.0f) {
        q2 = -q2;
        dot = -dot;
    }

    // Protection contre les imprécisions flottantes
    dot = glm::clamp(dot, -1.0f, 1.0f);

    // Si les quaternions sont extrêmement proches, basculer sur un LERP linéaire
    if (dot > 0.9995f) {
        return glm::normalize(glm::mix(q1, q2, t));
    }

    float theta = std::acos(dot);
    float sin_theta = std::sin(theta);

    return (std::sin((1.0f - t) * theta) * q1 + std::sin(t * theta) * q2) / sin_theta;
}
3. Résultats Après CorrectionEn réexécutant le même delta d'orientation ($2^\circ$ sous forme antipodale) avec les trois lignes activées :MétriqueSans CorrectionAvec Correction (3 lignes)Gain / NormalisationProduit Scalaire ($q_1 \cdot q_2$)$-0{,}9998$$+0{,}9998$Inversion de polaritéAngle Effectif ($\Delta \theta$)$358^\circ$$2^\circ$Trajectoire minimale rétablieVitesse Angulaire ($\omega$)$32\,223^\circ/\text{s}$$180^\circ/\text{s}$Division par ~180 de la vitesse4. Bilan Pédagogique et Impact VROrigine de l'anomalie : L'absence de vérification du produit scalaire force la trajectoire à contourner l'hypersphère au lieu de couper au plus court.Conséquence en Réalité Virtuelle : Un tel saut angulaire ($358^\circ$ en $11 \text{ ms}$) génère un flash visuel violent (camera snap), casse complètement le suivi de tête (head tracking) et provoque un choc vestibulaire immédiat chez l'utilisateur.