#include <stdio.h>
#include <math.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, S;

    if (argc != 3)
    {
        printf("Usage: ImageIn.pgm ImageOut \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    OCTET *ImgIn, *gradH, *gradV, *norme;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nTaille);

    // Allocatio tableaux
    allocation_tableau(gradH, OCTET, nTaille);
    allocation_tableau(gradV, OCTET, nTaille);
    allocation_tableau(norme, OCTET, nTaille);


    for (int y = 0; y < nH; y++)
    {
        for (int x = 0; x < nW; x++)
        {
            int indice = indicePixel(x,y,nW);
            // gradV
            // si bord : même pixel
            if (y == nH - 1)
            {
                gradV[indice] = ImgIn[indice];
            }
            else
            {
                // sinon soustraction du pixel suivant et du indice
                int suivant = indiceVoisinBas(x,y,nW);
                gradV[indice] = abs(ImgIn[suivant] - ImgIn[indice]);
            }
            // gradH
            if (x == nW - 1)
            {
                gradH[indice] = ImgIn[indice];
            }
            else
            {
                int suivant = indiceVoisinDroite(x,y,nW);
                gradH[indice] = abs(ImgIn[suivant] - ImgIn[indice]);
            }
            // norme
            norme[indice] = sqrt((pow(gradH[indice], 2) + pow(gradV[indice], 2)));
        }
    }
    // Creation nom fichiers
    char *nomN = (char *)malloc(strlen(cNomImgLue) + 8);  // ImageOut + -nm.pgm
    char *nomV = (char *)malloc(strlen(cNomImgLue) + 8); // ImageOut + -gv.pgm
    char *nomH = (char *)malloc(strlen(cNomImgLue) + 8); // ImageOut + -gh.pgm

    sprintf(nomN, "%s-nm.pgm", cNomImgEcrite);
    sprintf(nomV, "%s-gv.pgm", cNomImgEcrite);
    sprintf(nomH, "%s-gh.pgm", cNomImgEcrite);

    // Creation images
    ecrire_image_pgm(nomN, norme, nH, nW);
    ecrire_image_pgm(nomV, gradV, nH, nW);
    ecrire_image_pgm(nomH, gradH, nH, nW);

    // Libération
    free(ImgIn);
    free(gradV);
    free(gradH);
    free(norme);
    
    free(nomN);
    free(nomV);
    free(nomH);

    return 0;
}