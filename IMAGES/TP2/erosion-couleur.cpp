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

    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++)
        {
            int r = i * nW * 3 + j * 3;
            int g = r + 1;
            int b = r + 2;
            if (ImgIn[r] == 0 || ImgIn[g] == 0 || ImgIn[b] == 0 || ImgIn[(i - 1) * nW * 3 + j * 3] == 0 || ImgIn[(i - 1) * nW * 3 + j * 3 + 1] == 0 || ImgIn[(i - 1) * nW * 3 + j * 3 + 2] == 0 || ImgIn[(i + 1) * nW * 3 + j * 3] == 0 || ImgIn[(i + 1) * nW * 3 + j * 3 + 1] == 0 || ImgIn[(i + 1) * nW * 3 + j * 3 + 2] == 0 || ImgIn[i * nW * 3 + (j - 1) * 3] == 0 || ImgIn[i * nW * 3 + (j - 1) * 3 + 1] == 0 || ImgIn[i * nW * 3 + (j - 1) * 3 + 2] == 0 || ImgIn[i * nW * 3 + (j + 1) * 3] == 0 || ImgIn[i * nW * 3 + (j + 1) * 3 + 1] == 0 || ImgIn[i * nW * 3 + (j + 1) * 3 + 2] == 0)
            {
                ImgOut[r] = 0;
                ImgOut[g] = 0;
                ImgOut[b] = 0;
            }
            else
            {
                ImgOut[r] = 255;
                ImgOut[g] = 255;
                ImgOut[b] = 255;
            }
        }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);

    return 1;
}
