#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], emplacement[50];
    int nH, nW, nTaille, tabHisto[256], indice;

    if (argc != 4)
    {
        printf("Usage: ImageIn.pgm ligne/colonne indice\n");
        exit(1);
    }
    
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", emplacement);
    sscanf(argv[3], "%d", &indice);
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
    if (strcmp(emplacement, "ligne") == 0)
    {
        // recherche par ligne
        for (int j = 0; j < nW; j++)
        {
            tabHisto[ImgIn[indice * nW + j]]++;
        }
    }
    else if (strcmp(emplacement, "colonne") == 0)
    {
        // recherche par colonne
        for (int i = 0; i < nH; i++)
        {
            tabHisto[ImgIn[i * nW + indice]]++;
        }
    }
    else
    {
        printf("Usage: ImageIn.pgm ligne/colonne  0<=indice<=255\n");
        free(ImgIn);
        exit(1);
    }

    for (int m = 0; m < 256; m++)
    {
        if (tabHisto[m] != 0)
        {
            printf("%d\t%d\n", m, tabHisto[m]);
        }
    }
    free(ImgIn);
    return 1;
    
}
