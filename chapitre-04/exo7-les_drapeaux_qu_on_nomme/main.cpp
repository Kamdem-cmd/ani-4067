#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdint>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Table des drapeaux simples et composés
    std::map<std::string, uint32_t> flag_map = {
        {"NONE", 0U},
        {"RENDER2D", 1U},
        {"RENDER3D", 2U},
        {"TEXT", 4U},
        {"UI", 8U},
        {"SHADOW", 16U},
        {"POST_PROCESS", 32U},
        {"VFX", 64U},
        {"ANIMATION", 128U},
        {"OVERLAY", 256U},
        {"SIMULATION", 512U},
        {"OFFSCREEN", 1024U},
        {"RAYTRACING", 2048U},
        {"GPU_CULLING", 4096U},
        {"2D_ESSENTIALS", 1U | 4U},
        {"3D_BASE", 2U | 16U | 32U},
        {"DEBUG", 256U | 512U},
        {"ALL", 0xFFFFFFFFU}
    };

    uint32_t simple_flags[] = {1U, 2U, 4U, 8U, 16U, 32U, 64U, 128U, 256U, 512U, 1024U, 2048U, 4096U};

    int n = 0;
    if (!(std::cin >> n)) {
        // Fallback si pas d'entrée
        n = 0;
    }

    uint32_t final_val = 0U;
    std::vector<std::string> inconnus;

    if (n == 0) {
        final_val = 0xFFFFFFFFU;
    } else {
        for (int i = 0; i < n; ++i) {
            std::string name;
            std::cin >> name;

            auto it = flag_map.find(name);
            if (it != flag_map.end()) {
                final_val |= it->second;
            } else {
                inconnus.push_back(name);
            }
        }
    }

    // 1. Afficher les NOMS INCONNUS
    for (const auto& inc : inconnus) {
        std::cout << "INCONNU " << inc << "\n";
    }

    // 2. Afficher la VALEUR en décimal et en HEXA
    std::cout << "VALEUR " << final_val << "\n";
    std::cout << "HEXA 0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << final_val << std::dec << "\n";

    // 3. Vérifier les dépendances
    // TEXT exige RENDER2D
    if ((final_val & 4U) && !(final_val & 1U)) {
        std::cout << "MANQUE TEXT RENDER2D\n";
    }
    // UI exige RENDER2D et TEXT
    if (final_val & 8U) {
        if (!(final_val & 1U)) std::cout << "MANQUE UI RENDER2D\n";
        if (!(final_val & 4U)) std::cout << "MANQUE UI TEXT\n";
    }
    // SHADOW exige RENDER3D
    if ((final_val & 16U) && !(final_val & 2U)) {
        std::cout << "MANQUE SHADOW RENDER3D\n";
    }
    // OVERLAY exige RENDER2D et TEXT
    if (final_val & 256U) {
        if (!(final_val & 1U)) std::cout << "MANQUE OVERLAY RENDER2D\n";
        if (!(final_val & 4U)) std::cout << "MANQUE OVERLAY TEXT\n";
    }

    // 4. Compter les drapeaux simples allumés
    int allumes = 0;
    for (uint32_t flag : simple_flags) {
        if ((final_val & flag) == flag) {
            allumes++;
        }
    }
    int eteints = 13 - allumes;

    std::cout << "ALLUMES " << allumes << "\n";
    std::cout << "ETEINTS " << eteints << "\n";

    return 0;
}