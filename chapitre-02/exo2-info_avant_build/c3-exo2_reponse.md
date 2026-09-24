# ANI-4067 — Chapitre 02 : Exercice 2

---

### Informations
* **Date :** `2026-09-23`
* **Auteur :** Kamdem

---
# jenga info avant le build

**1. Resultat de la commande `jenga info` :**

```text
D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal>jenga info

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

========================= Jenga Workspace: MaSalleWks ==========================

Location: D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal
Entry file: D:\ENSPY\AN-GAP_4\teguis\ani-4067\chapitre-02\exo1-le_projet_minimal\MaSalleWks.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes: Windows
Target Architectures: x86_64


Projects
------------------------------------------------------------
Name      Kind         Language   Test   External
=================================================
MaSalle   ConsoleApp   C++        No     No


Available Toolchains
------------------------------------------------------------
Name                 Family        Target OS   Arch     Env
===============================================================
host-clang           clang         Windows     x86_64   mingw
host-gcc             gcc           Windows     x86_64   mingw
msvc                 msvc          Windows     x86_64   msvc
clang-mingw          clang         Windows     x86_64   mingw
mingw                gcc           Windows     x86_64   mingw
clang-cross-linux    clang         Linux       x86_64   gnu
android-ndk          android-ndk   Android     arm64    android
zig-linux-x86_64     clang         Linux       x86_64   gnu
zig-linux-x64        clang         Linux       x86_64   gnu
zig-windows-x86_64   clang         Windows     x86_64   mingw
zig-windows-x64      clang         Windows     x86_64   mingw
zig-macos-x86_64     clang         macOS       x86_64   gnu
zig-macos-arm64      clang         macOS       arm64    gnu
zig-ios-arm64        clang         iOS         arm64
zig-tvos-arm64       clang         tvOS        arm64
zig-watchos-arm64    clang         watchOS     arm64
zig-android-arm64    clang         Android     arm64    android
zig-web-wasm32       clang         Web         wasm32


Daemon
------------------------------------------------------------
Status: Not running
```
**2. Ce que la commande apprends de plus:**


Cette commande `jenga info` renseigne davantage sur:
--
**->** le nom du projet `MaSalle`; 

**->** le type de projet `ConsoleApp`;

**->** le langage de programmation `C++`; 

**->** Elle dit que les tests unitaires ne sont pas actifs `Test `qui contient `No`.

**->** Elle dit que le projet ne depend pas de librerie externe `Externam `qui contient `No`.

**->** Elle liste également les differentes `chaîne d'outils de compilation` installé.
