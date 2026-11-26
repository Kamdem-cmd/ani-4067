#include <iostream>
#include <string>
#include <vector>
#include <set>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int countA = 0;
    if (!(std::cin >> countA)) return 0;

    std::vector<std::string> listA(countA);
    std::set<std::string> setA;
    for (int i = 0; i < countA; ++i) {
        std::cin >> listA[i];
        setA.insert(listA[i]);
    }

    int countB = 0;
    std::cin >> countB;
    std::vector<std::string> listB(countB);
    std::set<std::string> setB;
    for (int i = 0; i < countB; ++i) {
        std::cin >> listB[i];
        setB.insert(listB[i]);
    }

    int deux = 0;
    for (const auto& name : listA) {
        if (setB.count(name)) {
            deux++;
        }
    }

    std::vector<std::string> seulA;
    for (const auto& name : listA) {
        if (!setB.count(name)) {
            seulA.push_back(name);
        }
    }

    std::vector<std::string> seulB;
    for (const auto& name : listB) {
        if (!setA.count(name)) {
            seulB.push_back(name);
        }
    }

    std::set<std::string> unionSet = setA;
    unionSet.insert(setB.begin(), setB.end());

    size_t differentes = seulA.size() + seulB.size();
    size_t totalUnique = unionSet.size();

    std::cout << "DEUX " << deux << "\n";
    for (const auto& name : seulA) {
        std::cout << "SEUL_A " << name << "\n";
    }
    for (const auto& name : seulB) {
        std::cout << "SEUL_B " << name << "\n";
    }
    std::cout << "DIFFERENTES " << differentes << " SUR " << totalUnique << "\n";

    return 0;
}