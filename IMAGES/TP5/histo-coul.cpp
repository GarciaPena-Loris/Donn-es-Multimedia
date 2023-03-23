#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250];
    int nH, nW, nTaille, *tabHistoR, *tabHistoG, *tabHistoB;

    if (argc != 2)
    {
        printf("Usage: ImageIn.pgm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    // allocation du tableau
    allocation_tableau(tabHistoR, int, 256);
    allocation_tableau(tabHistoG, int, 256);
    allocation_tableau(tabHistoB, int, 256);

    for (int i = 0; i < nTaille3; i += 3)
    {
        tabHistoR[ImgIn[i]]++;
        tabHistoG[ImgIn[i + 1]]++;
        tabHistoB[ImgIn[i + 2]]++;
    }

    // affichage
    for (int m = 0; m < 256; m++)
    {
        printf("%i %i %i %i\n", m, tabHistoR[m], tabHistoG[m], tabHistoB[m]);
    }

    free(ImgIn);
    free(tabHistoR);
    free(tabHistoG);
    free(tabHistoB);

    return 1;
}
