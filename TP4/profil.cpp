#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], emplacement[10];
    int nH, nW, nTaille, indice;

    if (argc != 4)
    {
        printf("Usage: ImageIn.pgm 'ligne'/'colonne' 0<=indice\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", emplacement);
    sscanf(argv[3], "%d", &indice);
    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    if (strcmp(emplacement, "ligne") == 0)
    {
        // recherche par ligne
        for (int j = 0; j < nW; j++)
        {
            printf("%d %d\n", j, ImgIn[indice * nW + j]);

        }
    }
    else if (strcmp(emplacement, "colonne") == 0)
    {
        // recherche par colonne
        for (int i = 0; i < nH; i++)
        {
            printf("%d %d\n", i, ImgIn[i * nW + indice]);
        }
    }
    else
    {
        printf("Usage: ImageIn.pgm 'ligne'/'colonne'  indice\n");
        free(ImgIn);
        exit(1);
    }

    free(ImgIn);
    return 1;
}
