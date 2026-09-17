# Compte-rendu d'exercice — Perception Relative de l'Échelle et Biais de Conception

---

## 1. Verbatims des Trois Observateurs (Tableau de Synthèse)

Les trois participants ont décrit la même salle d'étude sans connaître le facteur d'échelle qui leur était implicitement assigné par le point d'observation.

| Participant | Échelle Appliquée *(Masquée)* | Mots-clés & Verbatims Notés au Tableau | Niveau d'Abstraction Graphique |
| :--- | :---: | :--- | :--- |
| **Observateur A** | **Échelle Micro**<br>*(Detail / Texel Level)* | *"Grain du bois synthétique, rayures sur le tableau, poussière sur le projecteur, rainures de 2 mm du carrelage, bordures métalliques."* | **Matériaux & Shading :** Haute fréquence spectrale, résolution de texture (*Texel Density*), rugosité et détails de surface (*Normal Maps*). |
| **Observateur B** | **Échelle Humaine**<br>*(Gameplay / 1:1)* | *"Espace de 30 personnes, tables trop serrées, hauteur sous plafond étouffante, passage étroit entre les rangées, écran visible depuis le fond."* | **Pawn / Camera View :** Volumes d'interaction, cône de vision (*FOV*), navigation de l'utilisateur, dégagements et collisions. |
| **Observateur C** | **Échelle Macro**<br>*(World / Architectural)* | *"Boîte de 10×8×3 mètres, 2ème étage aile Est, connectée au couloir central par deux portes, orientée plein Nord avec 3 baies vitrées."* | **Scene Graph & Topology :** Boîte englobante (*AABB*), structures de partitionnement spatial, visibilité et portails de cellule (*Portals/Occlusion*). |

---

## 2. Analyse des Facteurs d'Échelle et Concepts Moteur

Cette expérience met en évidence trois représentations coexistantes d'un même monde dans un moteur 3D (`NkIDevice`) :

* **L'échelle topologique (Macro) :** Elle définit le positionnement relatif dans le monde via la scène graph (*Model Matrices*) et gère le culling géométrique.
* **L'échelle fonctionnelle (Humaine) :** Elle définit l'ergonomie spatiale. Un volume peut être mathématiquement correct tout en paraissant claustrophobique à cause de l'écartement de la caméra ou du champ de vision (*Field of View*).
* **L'échelle de fréquence visuelle (Micro) :** Elle apporte l'ancrage tactile. Sans micro-détails (imperfections, variations de rugosité), l'œil humain peine à évaluer la taille réelle d'un objet uniforme.

---

## 3. Pourquoi l'Auteur d'un Monde est le Plus Mal Placé pour en Juger l'Échelle

Le concepteur ou développeur d'un univers virtuel souffre de **trois biais cognitifs majeurs** qui altèrent irréversiblement sa perception de l'espace :

### A. Le Biais des Unités Arbitraires
L'auteur manipule des grandeurs numériques absolues dans l'éditeur (`1.0f = 1 mètre`). Ayant défini lui-même la matrice de transformation et la taille des maillages, son cerveau substitue la **mesure mathématique** (la valeur brute dans le fichier asset) à la **perception sensorielle** de l'espace.

### B. Le Piège de la Caméra d'Édition (`FlyCam`)
En phase de développement, l'auteur navigue avec une caméra libre sans contrainte physique, à une vitesse de déplacement arbitraire, sans hauteur d'œil fixe et souvent avec un champ de vision (*FOV*) déformé par la fenêtre de l'éditeur. L'échelle perçue en caméra libre n'a aucun rapport avec l'expérience incarnée à la première personne ou dans un casque VR.

### C. L'Absence d'Ancre de Référence Subconsciente
La perception humaine de l'échelle est exclusivement **comparative**. Un être humain n'évalue pas la taille d'une pièce en mètres, mais en la comparant inconsciemment à des repères du quotidien (*Scale Anchors*) : la hauteur d'une poignée de porte, la largeur d'une marche d'escalier, la texture d'un matériau. L'auteur, connaissant la structure globale du monde qu'il a conçu, oublie de fournir ces indices visuels indispensables aux observateurs extérieurs.

---

> **Conclusion pour le Moteur de Jeu :**
> 
> L'échelle d'un monde 3D n'est pas une propriété intrinsèque de la géométrie stockée en VRAM ; c'est une relation dynamique entre le repère de la caméra, les ancres visuelles du décor et la réponse comportementale du joueur.