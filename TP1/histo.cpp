#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250];
    int nH, nW, nTaille, tabHisto[256];

    if (argc != 2)
    {
        printf("Usage: ImageIn.pgm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);


    for (int u = 0; u < 256; u++)
    {
        tabHisto[u] = 0;
    }
    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++)
        {
            tabHisto[ImgIn[i * nW + j]]++;
        }
    for (int m = 0; m < 256; m++)
    {
        if (tabHisto[m] != 0)
        {
            printf("%d pixel de valeur: %d\n", tabHisto[m], m);
        }
    }
    free(ImgIn);

    return 1;
}
