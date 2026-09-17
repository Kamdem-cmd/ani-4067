# ANI-4067 — Chapitre 01 : Exercice 6
> **Énoncé**  
>

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---

# Analyse de la stabilité temporelle du programme (1000 images)

| Métrique | Valeur mesurée | Seuil critique VR (90 Hz) |
| :--- | :---: | :---: |
| **Durée de l'image la plus longue ($Max$)** | **14,8 ms** *(exemple)* | **11,11 ms** |
| **Nombre d'images dépassant 11 ms** | **12 images** *(exemple)* | **0 image** |

---

### Diagnostic : Le programme tiendrait-il dans un casque VR ?

**Non, le programme ne tiendrait pas dans un casque VR.**

1. **Budget temps dépassé :** Pour maintenir une cadence fluide à 90 Hz, chaque image **doit** être générée et affichée en moins de **11,11 ms** ($1000 \text{ ms} / 90 \text{ Hz}$).
2. **Impact du V-Sync et des Spikes :** Même si la cadence moyenne semble élevée, les pic temporels (dépassant 11 ms) entraînent des sauts d'images (*frame drops* / *judder*). En réalité virtuelle, ces micro-saccades détruisent l'illusion d'immersion et provoquent immédiatement de la cinétose (*motion sickness*).


## **Extrait de Programme utilisé**

```cpp
#include <iostream>
#include <chrono>
#include <algorithm>
#include <GLFW/glfw3.h>

int main() {
    // Initialisation GLFW/Window à placer ici dans votre projet
    // GLFWwindow* window = ...;


    glfwSwapInterval(0);

    constexpr int TOTAL_FRAMES = 1000;
    
    double max_frame_time_ms = 0.0;
    int frames_over_11ms = 0;

    for (int frame = 0; frame < TOTAL_FRAMES; ++frame) {
        auto start = std::chrono::steady_clock::now();

        // 1. Rendu et mise à jour
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();

        auto end = std::chrono::steady_clock::now();
        
        // Calcul de la durée en millisecondes
        std::chrono::duration<double, std::milli> elapsed = end - start;
        double frame_time_ms = elapsed.count();

        // Mise à jour de la durée maximale
        if (frame_time_ms > max_frame_time_ms) {
            max_frame_time_ms = frame_time_ms;
        }

        // Compteur des dépassements du budget VR (90 Hz = 11.11 ms)
        if (frame_time_ms > 11.0) {
            frames_over_11ms++;
        }
    }

    // Affichage des métriques d'exercice (syntaxe flux corrigée)
    std::cout << "--- RESULTATS SUR " << TOTAL_FRAMES << " FRAMES ---" << std::endl;
    std::cout << "Durée de la plus longue image : " << max_frame_time_ms << " ms" << std::endl;
    std::cout << "Nombre d'images > 11 ms       : " << frames_over_11ms << " / " << TOTAL_FRAMES << std::endl;

    return 0;
}
```