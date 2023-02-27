#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char **argv)
{
    // Gestion des arguments : on veut trois images d'entrée et une image de sortie.
    if (argc != 5)
    {
        printf("Usage: ImageIn1_Y.ppm ImageIn2_Cb.ppm ImageIn3_Cr.ppm ImageOut \n");
        exit(1);
    }

    // Definition des variables
    char cNomImgLue1[250], cNomImgLue2[250], cNomImgLue3[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    OCTET *Y, *Cb, *Cr, *R_Out, *G_Out, *B_Out, *ImgOut;

    sscanf(argv[1], "%s", cNomImgLue1);
    sscanf(argv[2], "%s", cNomImgLue2);
    sscanf(argv[3], "%s", cNomImgLue3);
    sscanf(argv[4], "%s", cNomImgEcrite);

    // Lecture des images d'entrée
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;

    allocation_tableau(Y, OCTET, nTaille);
    allocation_tableau(Cb, OCTET, nTaille);
    allocation_tableau(Cr, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, Y, nH * nW);
    lire_image_pgm(cNomImgLue2, Cb, nH * nW);
    lire_image_pgm(cNomImgLue3, Cr, nH * nW);

    // Allocation des images de sortie
    allocation_tableau(R_Out, OCTET, nTaille);
    allocation_tableau(G_Out, OCTET, nTaille);
    allocation_tableau(B_Out, OCTET, nTaille);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    for (int x = 0; x < nW; x++)
    {
        for (int y = 0; y < nW; y++)
        {
            int index = indicePixel(x, y, nW);
            int R = Y[index] + 1.402 * (Cr[index] - 128);
            int G = Y[index] - 0.34414 * (Cb[index] - 128) - 0.714414 * (Cr[index] - 128);
            int B = Y[index] + 1.772 * (Cb[index] - 128);

            R_Out[index] = R;
            G_Out[index] = G;
            B_Out[index] = B;

            ImgOut[indicePixel(x,y,nW,0)] = max(0, min(255, R));
            ImgOut[indicePixel(x,y,nW,1)] = max(0, min(255, G));
            ImgOut[indicePixel(x,y,nW,2)] = max(0, min(255, B));
        }
    }

    // Enregistrement des composante
    char *imgR = (char *)malloc(strlen(cNomImgEcrite) + 7);
    char *imG = (char *)malloc(strlen(cNomImgEcrite) + 7);
    char *imgB = (char *)malloc(strlen(cNomImgEcrite) + 7);

    sprintf(imgR, "%s_R.pgm", cNomImgEcrite);
    sprintf(imG, "%s_G.pgm", cNomImgEcrite);
    sprintf(imgB, "%s_B.pgm", cNomImgEcrite);

    ecrire_image_pgm(imgR, R_Out, nH, nW);
    ecrire_image_pgm(imG, G_Out, nH, nW);
    ecrire_image_pgm(imgB, B_Out, nH, nW);

    // Enregistrement image pgm
    char *imgA = (char *)malloc(strlen(cNomImgEcrite) + 7);

    sprintf(imgA, "%s_A.pgm", cNomImgEcrite);
    ecrire_image_ppm(imgA, ImgOut, nH, nW);

    // Liberation de tout le monde
    free(Y);
    free(Cb);
    free(Cr);
    free(R_Out);
    free(G_Out);
    free(B_Out);
    free(ImgOut);

    return 1;
}