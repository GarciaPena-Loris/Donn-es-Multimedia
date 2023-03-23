// flou-multiple.cpp : floute plusieurs fois une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <string.h>

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];

    if (argc != 3)
    {
        printf("Usage: chemin/vers/ImageIn.pgm ImageOut.pgm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    // 5 flou2
    char commande[500];
    sprintf(commande, "flou2.exe %s ../newImages/newImagesMult/imageFlouTampon.pgm", cNomImgLue);
    system(commande);
    system("flou2.exe ../newImages/newImagesMult/imageFlouTampon.pgm ../newImages/newImagesMult/imageFlouTampon2.pgm");
    system("flou2.exe ../newImages/newImagesMult/imageFlouTampon2.pgm ../newImages/newImagesMult/imageFlouTampon3.pgm");
    system("flou2.exe ../newImages/newImagesMult/imageFlouTampon3.pgm ../newImages/newImagesMult/imageFlouTampon4.pgm");
    system("flou2.exe ../newImages/newImagesMult/imageFlouTampon4.pgm ../newImages/newImagesMult/imageFlouTampon5.pgm");
    char commande2[500];
    sprintf(commande, "flou2.exe ../newImages/newImagesMult/imageFlouTampon5.pgm ../newImages/newImagesMult/%s", cNomImgEcrite);
    system(commande);

    return 1;
}
