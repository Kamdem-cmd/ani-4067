# ANI-4067 — Chapitre 02 : Exercice 4

**InformationsDate : 2026-09-24**

**Auteur : Kamdem**


## 1. Dépendance manquante dans le système de construction (Jenga)

Lorsqu'un module $A$ dépend d'un module $B$, mais que le module $B$ est retiré du fichier de configuration du projet (MaSalleWks.jenga), l'outil d'automatisation de la construction ne parvient pas à résoudre la référence vers le projet dépendant.

Output de la commande ``jenga build``

```powershell
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

[ERROR] Failed to resolve dependency 'ModuleB' for project 'ModuleA': Project 'ModuleB' not found in workspace.
Anayse de l'étape de la chaîne de constructionCet échec survient lors de la phase d'analyse de la configuration du système de construction (Build System / Jenga), avant même d'entamer les 4 étapes de la chaîne de compilation du C++.2. Dépendance manquante au niveau de la chaîne de compilation C++Si le projet $A$ inclut un fichier d'en-tête de $B$ (ex: #include "ModuleB.hpp") et utilise l'une de ses fonctions sans que $B$ ne soit lié/compilé :Cas A : Le fichier d'en-tête #include "ModuleB.hpp" est introuvableD:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>jenga build --config debug

...
╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: ModuleA                                                         Kind: STATIC_LIB   ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
x   [1/1] Failed: ModuleA.cpp
    ModuleA.cpp:2:10: fatal error: 'ModuleB.hpp' file not found
    #include "ModuleB.hpp"
             ^~~~~~~~~~~~~~
1 error generated.

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  x Build Failed                                                                 Time: 0.12s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘
Étape de la chaîne de construction : Préprocesseur (ou Préprocessing)Explication : C'est le préprocesseur qui gère la directive #include. Il parcourt les dossiers d'inclusion à la recherche du fichier d'en-tête spécifié et s'arrête en erreur s'il ne le trouve pas.Cas B : Le fichier d'en-tête est trouvé, mais la définition de la fonction manque à l'édition de liensD:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>jenga build --config debug

...
╔══════════════════════════════════════════════════════════════════════════════════════════════╗
║  Project: MaSalle                                                         Kind: CONSOLE_APP  ║
╚══════════════════════════════════════════════════════════════════════════════════════════════╝

ℹ Found 1 source file(s)
✓   [1/1] Compiled: main.cpp
ℹ Linking...
lld-link: error: undefined symbol: void __cdecl ModuleB::Fonction()
>>> referenced by main.cpp:8
>>>               src_main.obj:(main)
clang++: error: linker command failed with exit code 1 (use -v to see invocation)

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  x Build Failed                                                                 Time: 0.45s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘
Étape de la chaîne de construction : Édition de liens (Link / Linker)Explication : La compilation produit des fichiers objets (.obj), mais lors de l'assemblage final de l'exécutable, l'éditeur de liens (lld-link) ne trouve pas le code exécutable binaire correspondant au symbole/fonction déclarée.BilanPréprocesseur : Erreur levée si un fichier d'en-tête (#include) du second module est introuvable.Compilation / Assemblage : Vérifient la syntaxe et génèrent le code objet .obj.Édition de liens : Erreur levée (undefined symbol) si la fonction est déclarée mais que le fichier binaire/bibliothèque du second module n'est pas fourni à l'éditeur de liens.