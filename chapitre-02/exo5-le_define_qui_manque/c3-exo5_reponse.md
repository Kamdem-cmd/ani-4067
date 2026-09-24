ANI-4067 — Chapitre 02 : Exercice 5

Informations

Date : 2026-09-24

Auteur : Kamdem

1. Code source de test

Pour réaliser cette expérience, le fichier d'en-tête MaClasse.hpp utilise une directive de préprocesseur #ifdef ENABLE_FULL_CLASS pour basculer entre une classe complète et une coquille vide :

// MaClasse.hpp
#pragma once

#ifdef ENABLE_FULL_CLASS
class MaClasse {
public:
    void afficher() const;
};
#else
class MaClasse {}; // Coquille vide
#endif


Le programme principal main.cpp instancie cette classe et appelle sa méthode :

// main.cpp
#include "MaClasse.hpp"

int main() {
    MaClasse objet;
    objet.afficher();
    return 0;
}


2. Compilation AVEC le #define

En définissant la macro ENABLE_FULL_CLASS (par exemple via l'option du compilateur -DENABLE_FULL_CLASS), la classe est complète mais sa méthode afficher() n'est pas définie dans un fichier .cpp.

Output de la commande jenga build

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
lld-link: error: undefined symbol: public: void __cdecl MaClasse::afficher(void)const 
>>> referenced by main.cpp:5
>>>               src_main.obj:(main)
clang++: error: linker command failed with exit code 1 (use -v to see invocation)

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  x Build Failed                                                                 Time: 0.42s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  0/1
Time:           0.45s
Status:         x FAILED
════════════════════════════════════════════════════════════════════════════════


Analyse : La compilation de main.cpp réussit car le prototype de la fonction est bien déclaré. C'est l'Édition de liens (Linker) qui échoue car la méthode afficher() n'a pas de corps/définition dans les fichiers objets transmis.

3. Compilation SANS le #define

Sans la macro ENABLE_FULL_CLASS, le préprocesseur conserve la version "coquille vide" (class MaClasse {};). La méthode afficher() n'existe donc pas.

Output de la commande jenga build

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
x   [1/1] Failed: main.cpp
    main.cpp:5:11: error: no member named 'afficher' in 'MaClasse'
        objet.afficher();
        ~~~~~ ^
1 error generated.

┌──────────────────────────────────────────────────────────────────────────────────────────────┐
│  x Build Failed                                                                 Time: 0.15s  │
└──────────────────────────────────────────────────────────────────────────────────────────────┘

════════════════════════════════════════════════════════════════════════════════
                                BUILD COMPLETED
════════════════════════════════════════════════════════════════════════════════
Projects Built:  0/1
Time:           0.18s
Status:         x FAILED
════════════════════════════════════════════════════════════════════════════════


Analyse : L'erreur survient lors de la phase de Compilation. Le compilateur vérifie la syntaxe et la cohérence des types : il constate que le membre afficher n'existe tout simplement pas dans le type MaClasse.

Bilan et Diagnostic

Message avec #define (undefined symbol) : Erreur d'édition de liens (Linker).

Message sans #define (no member named '...') : Erreur de compilation (Compiler).

Diagnostic sans l'exercice :
Le message sans le #define (no member named 'afficher' in 'MaClasse') aurait été immédiatement compréhensible et diagnostiquable sans cet exercice, car il s'agit d'une erreur de compilation directe indiquant explicitement qu'un membre appelé n'existe pas dans la classe.

En revanche, le message d'édition de liens avec le #define (undefined symbol) aurait été plus déroutant sans expérience préalable : il survient plus tard dans le build et indique que la déclaration existe bien, mais que la définition binaire est absente, ce qui met en évidence la distinction essentielle entre déclaration et définition en C++.