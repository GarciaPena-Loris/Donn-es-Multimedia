#include <stdio.h>
#include "image_ppm.h"

#define min(a,b) (a < b ? a : b)
#define max(a,b) (a >= b ? a : b)


int main(int argc, char **argv) {
    // Gestion des arguments : on veut une image d'entrée et une image de sortie.
    if (argc != 5) {
        printf("Usage: ImageIn1_Y.ppm ImageIn2_Cb.ppm ImageIn3_Cr.ppm ImageOut.ppm \n"); 
        exit (1) ;
    }
    
    // Definition des variables
    char cNomImgLue1[250], cNomImgLue2[250], cNomImgLue3[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    OCTET *Y, *Cb, *Cr, *ImgOut;

    sscanf (argv[1],"%s",cNomImgLue1);
    sscanf (argv[2],"%s",cNomImgLue2);
    sscanf (argv[3],"%s",cNomImgLue3);
    sscanf (argv[4],"%s",cNomImgEcrite);


    // Lecture de l'image d'entrée     
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;

    allocation_tableau(Y, OCTET, nTaille);
    allocation_tableau(Cb, OCTET, nTaille);
    allocation_tableau(Cr, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, Y, nH * nW);
    lire_image_pgm(cNomImgLue2, Cb, nH * nW);
    lire_image_pgm(cNomImgLue3, Cr, nH * nW);
    
    // Allocation de l'image de sortie
    allocation_tableau(ImgOut, OCTET, nTaille3);

    for (int i = 0; i < nH; ++i) {
        for (int j = 0; j < nW; ++j) {
            int index = i*nW + j;

            int R = Y[index] + 1.402 * (Cr[index] - 128);
            int G = Y[index] - 0.34414 * (Cb[index] - 128) - 0.714414*(Cr[index] - 128);
            int B = Y[index] + 1.772 * (Cb[index] - 128);

            ImgOut[index*3] = max(0, min(255, R));
            ImgOut[index*3+1] = max(0, min(255, B));
            ImgOut[index*3+2] = max(0, min(255, G));
        }
    }


    // Enregistrement
    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);


    free(Y);
    free(Cb);
    free(Cr);
    free(ImgOut);

    return 1;
}