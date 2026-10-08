#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <algorithm>

// Renvoie l'ordre de priorité des APIs pour une plateforme donnée
std::vector<std::string> get_platform_order(const std::string& platform) {
    if (platform == "WINDOWS") {
        return {"VULKAN", "DX12", "DX11", "OPENGL", "SOFTWARE"};
    } else if (platform == "MACOS") {
        return {"METAL", "OPENGL", "SOFTWARE"};
    } else if (platform == "IOS") {
        return {"METAL", "SOFTWARE"};
    } else if (platform == "ANDROID") {
        return {"VULKAN", "OPENGL", "SOFTWARE"};
    } else {
        // Toute autre plateforme
        return {"VULKAN", "OPENGL", "SOFTWARE"};
    }
}

// Convertit le code API en nom d'affichage lisible
std::string get_display_name(const std::string& api) {
    if (api == "VULKAN") return "Vulkan";
    if (api == "DX12") return "DirectX 12";
    if (api == "DX11") return "DirectX 11";
    if (api == "OPENGL") return "OpenGL";
    if (api == "METAL") return "Metal";
    if (api == "SOFTWARE") return "Software";
    return api;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    if (!(std::cin >> n)) {
        std::cout << "IGNOREES 0\n";
        std::cout << "LOGICIEL 0\n";
        std::cout << "DIFFERENTES 0\n";
        return 0;
    }

    int total_ignorees = 0;
    int total_logiciel = 0;
    std::unordered_set<std::string> display_names_used;

    for (int i = 0; i < n; ++i) {
        std::string machine_name, platform;
        int k = 0;
        std::cin >> machine_name >> platform >> k;

        std::unordered_set<std::string> available_apis;
        std::vector<std::string> platform_order = get_platform_order(platform);
        std::unordered_set<std::string> valid_platform_apis(platform_order.begin(), platform_order.end());

        for (int j = 0; j < k; ++j) {
            std::string api;
            std::cin >> api;
            available_apis.insert(api);

            // Compter les APIs non reconnues par la plateforme
            if (valid_platform_apis.find(api) == valid_platform_apis.end()) {
                total_ignorees++;
            }
        }

        // Sélection de la première API valide selon l'ordre de priorité
        std::string chosen_api = "SOFTWARE";
        for (const auto& api : platform_order) {
            if (available_apis.find(api) != available_apis.end()) {
                chosen_api = api;
                break;
            }
        }

        if (chosen_api == "SOFTWARE") {
            total_logiciel++;
        }

        std::string readable_name = get_display_name(chosen_api);
        display_names_used.insert(readable_name);

        std::cout << machine_name << " " << readable_name << "\n";
    }

    std::cout << "IGNOREES " << total_ignorees << "\n";
    std::cout << "LOGICIEL " << total_logiciel << "\n";
    std::cout << "DIFFERENTES " << display_names_used.size() << "\n";

    return 0;
}