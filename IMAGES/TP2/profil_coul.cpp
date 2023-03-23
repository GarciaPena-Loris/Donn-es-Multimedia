#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], emplacement[10];
    int nH, nW, nTaille, nTaille3, indice;

    if (argc != 4)
    {
        printf("Usage: ImageIn.ppm 'ligne'/'colonne' 0<=indice\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", emplacement);
    sscanf(argv[3], "%d", &indice);
    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    if (strcmp(emplacement, "ligne") == 0)
    {
        // recherche par ligne
        for (int x = 0; x < nW; x++)
        {
            printf("%d %d %d %d\n", x, ImgIn[ImgIn[indicePixel(x, indice, nW, 0)]], ImgIn[indicePixel(x, indice, nW, 1)], ImgIn[indicePixel(x, indice, nW, 2)]);
        }
    }
    else if (strcmp(emplacement, "colonne") == 0)
    {
        // recherche par colonne
        for (int y = 0; y < nH; y++)
        {
            printf("%d %d %d %d\n", y, ImgIn[indicePixel(indice, y, nW, 0)], ImgIn[indicePixel(indice, y, nW, 1)], ImgIn[indicePixel(indice, y, nW, 2)]);
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
