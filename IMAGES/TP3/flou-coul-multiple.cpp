// flou-coul-multiple.cpp : floute plusieurs fois une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <string.h>

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];

    if (argc != 3)
    {
        printf("Usage: chemin/vers/ImageIn.ppm ImageOut.ppm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    // 5 flou-coul
    char commande[500];
    sprintf(commande, "flou-coul.exe %s ../newImages/newImagesMult/imageFlouCoulTampon1.ppm", cNomImgLue);
    system(commande);
    system("flou-coul.exe ../newImages/newImagesMult/imageFlouCoulTampon1.ppm ../newImages/newImagesMult/imageFlouCoulTampon2.ppm");
    system("flou-coul.exe ../newImages/newImagesMult/imageFlouCoulTampon2.ppm ../newImages/newImagesMult/imageFlouCoulTampon3.ppm");
    system("flou-coul.exe ../newImages/newImagesMult/imageFlouCoulTampon3.ppm ../newImages/newImagesMult/imageFlouCoulTampon4.ppm");
    system("flou-coul.exe ../newImages/newImagesMult/imageFlouCoulTampon4.ppm ../newImages/newImagesMult/imageFlouCoulTampon5.ppm");
    char commande2[500];
    sprintf(commande, "flou-coul.exe ../newImages/newImagesMult/imageFlouCoulTampon5.ppm ../newImages/newImagesMult/%s", cNomImgEcrite);
    system(commande);

    return 1;
}
