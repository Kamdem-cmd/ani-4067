#include <iostream>
#include <string>

class Session {
private:
    bool ouverte_ = false;
    long long debuts_ = 0;
    long long fins_ = 0;
    long long refus_ = 0;

public:
    bool Wait() {
        if (ouverte_) {
            refus_++;
            return false;
        }
        return true;
    }

    bool Begin() {
        if (ouverte_) {
            refus_++;
            return false;
        }
        ouverte_ = true;
        debuts_++;
        return true;
    }

    bool End() {
        if (!ouverte_) {
            refus_++;
            return false;
        }
        ouverte_ = false;
        fins_++;
        return true;
    }

    long long debuts() const { return debuts_; }
    long long fins() const { return fins_; }
    long long refus() const { return refus_; }
    bool estOuverte() const { return ouverte_; }
};

class GardeTrame {
private:
    Session& session_;
    bool actif_;

public:
    GardeTrame(Session& s, bool beginOk) : session_(s), actif_(beginOk) {}
    GardeTrame(const GardeTrame&) = delete;
    GardeTrame& operator=(const GardeTrame&) = delete;

    ~GardeTrame() {
        if (actif_) {
            session_.End();
        }
    }
};

void simulerSansGarde(const std::string& signes) {
    Session s;
    long long images = 0;

    for (char c : signes) {
        bool waitOk = s.Wait();
        s.Begin();

        char chemin = c;
        if (!waitOk && (chemin == 'N' || chemin == 'C')) {
            chemin = 'V';
        }

        if (chemin == 'C') {
            continue;
        }
        if (chemin == 'R') {
            break;
        }
        if (chemin == 'N') {
            images++;
        }

        s.End();
    }

    std::cout << "SANS DEBUTS " << s.debuts()
              << " FINS " << s.fins()
              << " REFUS " << s.refus()
              << " IMAGES " << images
              << " OUVERTE " << (s.estOuverte() ? "OUI" : "NON") << "\n";
}

void simulerAvecGarde(const std::string& signes) {
    Session s;
    long long images = 0;

    for (char c : signes) {
        bool waitOk = s.Wait();
        bool beginOk = s.Begin();
        GardeTrame garde(s, beginOk);

        char chemin = c;
        if (!waitOk && (chemin == 'N' || chemin == 'C')) {
            chemin = 'V';
        }

        if (chemin == 'C') {
            continue;
        }
        if (chemin == 'R') {
            break;
        }
        if (chemin == 'N') {
            images++;
        }
    }

    std::cout << "AVEC DEBUTS " << s.debuts()
              << " FINS " << s.fins()
              << " REFUS " << s.refus()
              << " IMAGES " << images
              << " OUVERTE " << (s.estOuverte() ? "OUI" : "NON") << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T = 0;
    if (!(std::cin >> T)) return 0;

    std::string signes;
    if (T > 0) {
        std::cin >> signes;
    }

    simulerSansGarde(signes);
    simulerAvecGarde(signes);

    return 0;
}