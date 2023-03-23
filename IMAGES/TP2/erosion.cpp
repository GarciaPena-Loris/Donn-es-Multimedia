// erosion.cpp :

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;

    if (argc != 3)
    {
        printf("Usage: ImageIn.pgm ImageOut.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    for (int y = 0; y < nH; y++)
    {
        for (int x = 0; x < nW; x++)
        {

            if (pixelBlancVoisinage(ImgIn, x, y, nW, nH) == 1)
            {
                // la valeur du pixel de sortie à 255
                ImgOut[indicePixel(x, y, nW)] = 255;
            }
            else
            {
                // la valeur du pixel de sortie à 0
                ImgOut[indicePixel(x, y, nW)] = 0;
            }
        }
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);

    return 1;
}
