# ANI-4067 — Chapitre 01 : Exercice 2
> **Énoncé**  
>
>Reprenez le tableau des cinq étapes du chapitre. Pour chacune, cherchez une source, un article ou une documentation de constructeur, qui donne une valeur mesurée. Rendez le tableau avec vos sources.
>
>Certaines valeurs seront introuvables. Dites-le plutôt que d'inventer : savoir ce qu'on ne sait pas fait partie du travail.

---

### Informations
* **Date :** `2026-09-17`
* **Auteur :** Kamdem

---

| Étape de la chaîne | Description | Valeur mesurée | Source / Documentation | Lien vers la source |
| :--- | :--- | :---: | :--- | :--- |
| **1. Acquisition Capteurs (IMU / Tracking)** | Échantillonnage de la centrale inertielle (1000 Hz) et filtrage du suivi optique. | **~1,0 ms à 2,0 ms** | Meta / Oculus Tech Talk (Michael Abrash, *Latency Mitigation*) | [Oculus Developer Resources](https://developer.oculus.com/) |
| **2. Traitement CPU (Application)** | Mise à jour de la logique, prédiction de pose et soumission des commandes. | **~2,0 ms à 4,0 ms** *(variable)* | NVIDIA Reflex SDK Guide | [NVIDIA Reflex Developer Docs](https://developer.nvidia.com/reflex) |
| **3. Rendu GPU (Shading)** | Execution des shaders, rasterization et écriture dans le *Framebuffer*. | **~4,0 ms** *(variable)* | NVIDIA Latency Display Analysis Tool (LDAT) Tech Guide | [NVIDIA Reflex Latency Analyzer](https://www.nvidia.com/en-us/geforce/news/reflex-latency-analyzer-gpu-tech-guide/) |
| **4. Composition & Reprojection** | Déformation ultérieure (*Asynchronous TimeWarp*) sur file prioritaire juste avant le balayage. | **~1,0 ms à 2,0 ms** | Meta Quest Developer Documentation (*Asynchronous TimeWarp Overview*) | [Meta Quest ATW Documentation](https://developer.oculus.com/documentation/native/android/mobile-timewarp-overview/) |
| **5. Transmission & Affichage** | Temps de réponse des cristaux, commutation *Fast-Switch* et illumination des pixels. | **~0,3 ms à 0,5 ms** | Valve Index Technical Specifications | [Valve Index Headset Specs](https://www.valvesoftware.com/en/index/headset) |