#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKLogger/NkLog.h"
#include "NKImage/Core/NkImage.h"
#include <cstdio>

NKENTSEU_DEFINE_APP_DATA(([](){
    nkentseu::NkAppData d{};
    d.appName = "FormatsImage";
    d.appVersion = "1.0.0";
    return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
    // 1. Charger la photographie source (ex: "brick.png")
    // Assurez-vous d'avoir placé une image "brick.png" dans le répertoire de travail.
    nkentseu::NkImage photo;
    if (!photo.Load("brick.png")) {
        logger.Error("Erreur : Impossible de charger la photo source 'brick.png'");
        return -1;
    }
    logger.Info("Photo source chargee avec succes : %d x %d pixels", photo.Width(), photo.Height());

    // 2. Faire écrire la photo dans cinq formats différents par le moteur
    const char* pathPNG = "mur_out.png";
    const char* pathJPG = "mur_out.jpg";
    const char* pathBMP = "mur_out.bmp";
    const char* pathTGA = "mur_out.tga";
    const char* pathQOI = "mur_out.qoi";

    photo.SavePNG(pathPNG);
    photo.SaveJPEG(pathJPG, 90);
    photo.SaveBMP(pathBMP);
    photo.SaveTGA(pathTGA);
    photo.SaveQOI(pathQOI);

    // 3. Fonction utilitaire pour récupérer la taille d'un fichier sur disque en octets
    auto getFileSize = [](const char* filename) -> long {
        FILE* f = std::fopen(filename, "rb");
        if (!f) return -1;
        std::fseek(f, 0, SEEK_END);
        long size = std::ftell(f);
        std::fclose(f);
        return size;
    };

    // 4. Relire chacun des cinq fichiers dans un nouveau NkImage, mesurer et afficher
    const char* formatsNames[] = {"png", "jpeg", "bmp", "tga", "qoi"};
    const char* formatsPaths[] = {pathPNG, pathJPG, pathBMP, pathTGA, pathQOI};

    for (int i = 0; i < 5; ++i) {
        long sizeBytes = getFileSize(formatsPaths[i]);
        nkentseu::NkImage imgRelue;
        if (sizeBytes >= 0 && imgRelue.Load(formatsPaths[i])) {
            std::printf("%s : %ld octets, %d x %d\n", formatsNames[i], sizeBytes, imgRelue.Width(), imgRelue.Height());
        } else {
            std::printf("%s : %ld octets, illisible\n", formatsNames[i], sizeBytes);
        }
    }

    return 0;
}