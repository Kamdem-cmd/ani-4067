# ANI-4067 — Chapitre 02 : Exercice 3

---

### Informations
* **Date :** `2026-09-24`
* **Auteur :** Kamdem

---
## Comparaison Debug & Release

# **Build en `Debug`**

```text
D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>jenga build --config debug

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

Loading workspace...

Configuration: debug
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (1 projects):
  1. MaSalle [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MaSalle                                                         Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Bin\Debug-Windows\MaSalle\MaSalle.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 0.87s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  1/1
Time:           0.88s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```

```text
D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>dir D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal\Build\Bin\Debug-Windows\MaSalle
 Le volume dans le lecteur D s’appelle Data
 Le numéro de série du volume est 18DD-6CC4

 Répertoire de D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal\Build\Bin\Debug-Windows\MaSalle

24/09/2026  12:25    <DIR>          .
24/09/2026  12:25    <DIR>          ..
24/09/2026  12:25           143 656 MaSalle.exe
               1 fichier(s)          143 656 octets
               2 Rép(s)  24 154 955 776 octets libres

```

# **Build en `Release`**

```text
D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>jenga build --config release

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                ██╗███████╗███╗   ██╗ ██████╗  █████╗             ║
║                ██║██╔════╝████╗  ██║██╔════╝ ██╔══██╗            ║
║                ██║█████╗  ██╔██╗ ██║██║  ███╗███████║            ║
║           ██   ██║██╔══╝  ██║╚██╗██║██║   ██║██╔══██║            ║
║           ╚█████╔╝███████╗██║ ╚████║╚██████╔╝██║  ██║            ║
║            ╚════╝ ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝            ║
║                                                                  ║
║             Multi-platform C/C++ Build System v2.8.0             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝

Loading workspace...

Configuration: release
Target:        Windows x86_64
Toolchain:     clang-mingw

Build Order (1 projects):
  1. MaSalle [CONSOLE_APP]


╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MaSalle                                                         Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
✓ Built: Build\Bin\release-Windows\MaSalle\MaSalle.exe

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  ✓ Build Successful                                                             Time: 5.31s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  1/1
Time:           5.36s
Status:         ✓ SUCCESS
════════════════════════════════════════════════════════════════════════════════
```

```text
D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>dir D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal\Build\Bin\release-Windows\MaSalle
 Le volume dans le lecteur D s’appelle Data
 Le numéro de série du volume est 18DD-6CC4

 Répertoire de D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal\Build\Bin\release-Windows\MaSalle

24/09/2026  12:20    <DIR>          .
24/09/2026  12:20    <DIR>          ..
24/09/2026  12:20           143 656 MaSalle.exe
               1 fichier(s)          143 656 octets
               2 Rép(s)  24 154 955 776 octets libres
```
## Bilan 

* **Temps de construction en Debug :** `0.88s`
* **Taille en Debug :** `143 656 octets`
* **Temps de construction en Release :** `5.36s`
* **Taille en Release :** `143 656 octets`

Ils ont tout deux la même taille mais avec des temps de constructions differents, le `Release` met plus de temps à construire que le `Debug`.