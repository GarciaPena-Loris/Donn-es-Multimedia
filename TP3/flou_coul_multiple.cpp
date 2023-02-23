// flou_multiple.cpp : floute plusieurs fois une image en niveau de gris
#include <stdio.h>
#include "image_ppm.h"
#include <string.h>

int main(int argc, char* argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, S;

    if (argc != 3) {
        printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);

    //5 filtre_flou2
    char commande[500];
    sprintf(commande, "../bin/flou_coul %s ../newImages/newImagesMult/imageFlouCoulTampon.pgm", cNomImgLue);
    system(commande);
    system("../bin/flou_coul ../newImages/newImagesMult/imageFlouCoulTampon.pgm ../newImages/newImagesMult/imageFlouCoulTampon2.pgm");
    system("../bin/flou_coul ../newImages/newImagesMult/imageFlouCoulTampon2.pgm ../newImages/newImagesMult/imageFlouCoulTampon3.pgm");
    system("../bin/flou_coul ../newImages/newImagesMult/imageFlouCoulTampon3.pgm ../newImages/newImagesMult/imageFlouCoulTampon4.pgm");
    system("../bin/flou_coul ../newImages/newImagesMult/imageFlouCoulTampon4.pgm ../newImages/newImagesMult/imageFlouCoulTampon5.pgm");
    char commande2[500];
    sprintf(commande, "../bin/flou_coul ../newImages/newImagesMult/imageFlouCoulTampon5.pgm ../newImages/newImagesMult/%s", cNomImgEcrite);
    system(commande);

    return 1;
}
