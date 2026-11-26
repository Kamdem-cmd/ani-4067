#include <iostream>
#include <string>
#include <vector>
#include <cctype>

long long parseNumber(const std::string& s, size_t& pos, bool& hasDigits) {
    while (pos < s.size() && s[pos] == ' ') {
        pos++;
    }
    int sign = 1;
    if (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
        if (s[pos] == '-') sign = -1;
        pos++;
    }
    long long val = 0;
    hasDigits = false;
    while (pos < s.size() && std::isdigit(static_cast<unsigned char>(s[pos]))) {
        hasDigits = true;
        val = val * 10 + (s[pos] - '0');
        pos++;
    }
    return hasDigits ? (sign * val) : 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string backend;
    long long hz = 0;
    std::string demi, profondeur, liste;
    long long latence = 0;

    if (!(std::cin >> backend >> hz >> demi >> profondeur >> liste >> latence)) return 0;

    std::string dummy;
    std::getline(std::cin, dummy);

    std::string loader;
    std::getline(std::cin, loader);

    std::string poseLine;
    std::getline(std::cin, poseLine);

    bool poseFixe = false;
    long long poseVals[5] = {0, 0, 0, 0, 0};
    if (poseLine.rfind("FIXE", 0) == 0) {
        poseFixe = true;
        size_t pos = 4;
        for (int k = 0; k < 5; ++k) {
            bool ok = false;
            poseVals[k] = parseNumber(poseLine, pos, ok);
        }
    }

    int M = 0;
    std::cin >> M;
    std::getline(std::cin, dummy);

    long long appliquees = 0;
    long long ignorees = 0;

    for (int m = 0; m < M; ++m) {
        std::string line;
        std::getline(std::cin, line);

        size_t eq = line.find('=');
        if (eq == std::string::npos) {
            ignorees++;
            continue;
        }

        std::string nom = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        if (val.empty()) {
            ignorees++;
            continue;
        }

        if (nom == "NK_XR_BACKEND") {
            if (val == "openxr") {
                backend = "OPENXR";
                appliquees++;
            } else {
                ignorees++;
            }
        } else if (nom == "NK_XR_HZ") {
            size_t pos = 0;
            bool ok = false;
            hz = parseNumber(val, pos, ok);
            appliquees++;
        } else if (nom == "NK_XR_SIM_LATENCY_MS") {
            size_t pos = 0;
            bool ok = false;
            latence = parseNumber(val, pos, ok);
            appliquees++;
        } else if (nom == "NK_XR_HALF_RATE" || nom == "NK_XR_DEPTH_LAYER" || nom == "NK_XR_LIST_EXT") {
            std::string etat = (val[0] == '0' || val == "false") ? "NON" : "OUI";
            if (nom == "NK_XR_HALF_RATE") demi = etat;
            else if (nom == "NK_XR_DEPTH_LAYER") profondeur = etat;
            else liste = etat;
            appliquees++;
        } else if (nom == "NK_XR_OPENXR_LOADER") {
            loader = val;
            appliquees++;
        } else if (nom == "NK_XR_SIM_POSE") {
            size_t pos = 0;
            long long nums[5];
            int count = 0;
            while (count < 5 && pos < val.size()) {
                bool hasDigits = false;
                long long v = parseNumber(val, pos, hasDigits);
                if (!hasDigits) break;
                nums[count++] = v;
                while (pos < val.size() && (val[pos] == ',' || val[pos] == ' ')) {
                    pos++;
                }
            }
            if (count == 5) {
                poseFixe = true;
                for (int k = 0; k < 5; ++k) poseVals[k] = nums[k];
                appliquees++;
            } else {
                ignorees++;
            }
        } else {
            ignorees++;
        }
    }

    std::cout << "BACKEND " << backend << "\n";
    std::cout << "HZ " << hz << "\n";
    std::cout << "DEMI " << demi << "\n";
    std::cout << "PROFONDEUR " << profondeur << "\n";
    std::cout << "LISTE " << liste << "\n";
    std::cout << "LOADER " << loader << "\n";
    std::cout << "LATENCE " << latence << "\n";
    if (!poseFixe) {
        std::cout << "POSE LIBRE\n";
    } else {
        std::cout << "POSE FIXE " << poseVals[0] << " " << poseVals[1] << " "
                  << poseVals[2] << " " << poseVals[3] << " " << poseVals[4] << "\n";
    }
    std::cout << "APPLIQUEES " << appliquees << "\n";
    std::cout << "IGNOREES " << ignorees << "\n";

    return 0;
}