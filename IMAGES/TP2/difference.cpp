// difference.cpp :

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue1[250], cNomImgLue2[250], cNomImgEcrite[250];
    int nH1, nW1, nTaille1, nH2, nW2, nTaille2;

    if (argc != 4)
    {
        printf("Usage: ImageIn1.pgm ImageIn2.pgm  ImageOut.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue1);
    sscanf(argv[2], "%s", cNomImgLue2);
    sscanf(argv[3], "%s", cNomImgEcrite);

    OCTET *ImgIn1, *ImgIn2, *ImgOut;

    // image 1
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH1, &nW1);
    nTaille1 = nH1 * nW1;

    // image 2
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue2, &nH2, &nW2);
    nTaille2 = nH2 * nW2;

    if (nH1 != nH2 || nW1 != nW2) {
        printf("Usage: Il faut que les deux images ai la même taille. \n");
        exit(1);
    }

    // in 1
    allocation_tableau(ImgIn1, OCTET, nTaille1);
    lire_image_pgm(cNomImgLue1, ImgIn1, nH1 * nW1);

    // in 2
    allocation_tableau(ImgIn2, OCTET, nTaille2);
    lire_image_pgm(cNomImgLue2, ImgIn2, nH2 * nW2);

    // out
    allocation_tableau(ImgOut, OCTET, nTaille2);


    for (int i = 0; i < nH1; i++)
        for (int j = 0; j < nW1; j++)
        {
            if (ImgIn1[i * nW1 + j] == ImgIn2[i * nW2 + j])
            {
                ImgOut[i * nW1 + j] = 255;
            }
            else
            {
                ImgOut[i * nW1 + j] = 0;
            }
        }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH1, nW1);
    
    free(ImgIn1);
    free(ImgIn2);
    free(ImgOut);

    return 1;
}
