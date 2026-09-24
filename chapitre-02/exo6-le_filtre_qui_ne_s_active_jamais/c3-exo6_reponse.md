ANI-4067 — Chapitre 02 : Exercice 6

Informations

Date : 2026-09-24

Auteur : Kamdem

1. Fichier de projet (MaSalleWks.jenga)

Nous configurons un filtre dans le fichier de build Jenga.

Lorsque la condition pointe vers system:macosx (fausse sur notre machine Windows x86_64), la définition MA_DEFINITION n'est pas appliquée.

Lorsque nous changeons le filtre pour system:windows (vraie sur notre machine), la définition MA_DEFINITION est injectée lors de la compilation.

-- MaSalleWks.jenga

workspace "MaSalleWks"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "MaSalle"
    kind "ConsoleApp"
    language "C++"
    targetdir "Build/Bin/%{cfg.buildcfg}-%{cfg.system}"
    objdir "Build/Obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}"

    files { "MaSalle/src/**.cpp" }

    -- Cas 1 : Condition FAUSSE sur Windows (Filtre ciblant macOS)
    filter "system:macosx"
        defines { "MA_DEFINITION" }

    -- Cas 2 : Condition VRAIE sur Windows (Pour la seconde preuve)
    -- filter "system:windows"
    --     defines { "MA_DEFINITION" }


2. Code source du programme (main.cpp)

Comme les commandes jenga info et jenga build --verbose n'affichent pas la liste des pré-définitions injectées, nous laissons le programme vérifier la macro via le préprocesseur :

// MaSalle/src/main.cpp
#include <iostream>

int main() {
#ifdef MA_DEFINITION
    std::cout << "[RESULTAT] La definition MA_DEFINITION EST PRESENTE !" << std::endl;
#else
    std::cout << "[RESULTAT] La definition MA_DEFINITION EST ABSENTE !" << std::endl;
#endif
    return 0;
}


3. Preuve 1 : Condition Fausse (system:macosx)

Dans cette configuration, le filtre cible macOS alors que nous compilons sur Windows. La définition ne doit pas être appliquée.

Compilation et Exécution

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
│  ✓ Build Successful                                                             Time: 0.82s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>Build\Bin\Debug-Windows\MaSalle\MaSalle.exe
[RESULTAT] La definition MA_DEFINITION EST ABSENTE !


4. Preuve 2 : Condition Vraie (system:windows)

Nous modifions le fichier .jenga pour que le filtre corresponde au système hôte (filter "system:windows").

Compilation et Exécution

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
│  ✓ Build Successful                                                             Time: 0.85s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>Build\Bin\Debug-Windows\MaSalle\MaSalle.exe
[RESULTAT] La definition MA_DEFINITION EST PRESENTE !


Bilan

Premier enseignement : L'outil d'inspection du système de build (jenga info / jenga build --verbose) ne reflète pas toujours les drapeaux de préprocesseur ou les définitions passées au compilateur. Il ne faut donc pas compter exclusivement sur l'outil de build pour diagnostiquer la présence de macros.

Solution : Faire s'exprimer le programme lui-même à l'aide de directives #ifdef / #ifndef permet de vérifier sans ambiguïté la prise en compte ou le rejet des filtres de configuration.