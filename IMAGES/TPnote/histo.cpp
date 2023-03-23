#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250];
    int nH, nW, nTaille, *tabHisto;

    if (argc != 2)
    {
        printf("Usage: ImageIn.pgm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    // allocation du tableau
    allocation_tableau(tabHisto, int, 256);

    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++)
        {
            tabHisto[ImgIn[i * nW + j]]++;
        }
    for (int m = 0; m < 256; m++)
    {
        printf("%d %d\n", m, tabHisto[m]);
    }

    free(ImgIn);
    free(tabHisto);

    return 1;
}
