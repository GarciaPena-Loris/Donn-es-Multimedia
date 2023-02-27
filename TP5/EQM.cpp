#include <stdio.h>
#include <math.h>
#include "image_ppm.h"

int main(int argc, char **argv)
{
    // Gestion des arguments : on veut deux images d'entrée

    if (argc != 3)
    {
        printf("Usage: ImageIn1.pgm ImageIn2.pgm \n");
        exit(1);
    }

    // Deifinition des variables
    char cNomImgLue1[250], cNomImgLue2[250];
    int nH, nW, nTaille;
    OCTET *ImgIn1, *ImgIn2;

    sscanf(argv[1], "%s", cNomImgLue1);
    sscanf(argv[2], "%s", cNomImgLue2);

    // Lecture de l'image d'entrée 1
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn1, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, ImgIn1, nTaille);

    // Lecture de l'image d'entrée 2
    allocation_tableau(ImgIn2, OCTET, nTaille);
    lire_image_pgm(cNomImgLue2, ImgIn2, nTaille);

    // Calcul EQM
    double EQM = 0;
    for (int x = 0; x < nW; ++x)
    {
        for (int y = 0; y < nH; ++y)
        {
            int index = indicePixel(x, y, nW);
            EQM += pow(ImgIn1[index] - ImgIn2[index], 2);
        }
    }
    printf("EQM : %f\n", EQM / nTaille);

    // On libere
    free(ImgIn1);
    free(ImgIn2);

    return 1;
}