#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250];
    int nH, nW, nTaille, tabHistoR[256], tabHistoG[256], tabHistoB[256];

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
    allocation_tableau(ImgOut, OCTET, nTaille3);

    for (int u = 0; u < 256; u++)
    {
        tabHistoR[u] = 0;
        tabHistoG[u] = 0;
        tabHistoB[u] = 0;
    }
    for (int i = 0; i < nTaille3; i += 3)
    {
        tabHistoR[ImgIn[i]]++;
        tabHistoG[ImgIn[i + 1]]++;
        tabHistoB[ImgIn[i + 2]]++;
    }

    // affichage
    for (int m = 0; m < 256; m++)
    {
        if (tabHistoR[m] != 0)
        {
            printf("%d pixels rouge de valeur: %d\n", tabHistoR[m], m);
        }
        if (tabHistoG[m] != 0)
        {
            printf("%d pixels vert de valeur: %d\n", tabHistoG[m], m);
        }
        if (tabHistoB[m] != 0)
        {
            printf("%d pixels bleu de valeur: %d\n", tabHistoB[m], m);
        }
        printf("--------------------------------\n");
    }
    free(ImgIn);

    return 1;
}
