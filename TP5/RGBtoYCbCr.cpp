#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char **argv) {
    // Gestion des arguments : on veut une image d'entrée et une image de sortie.
    if (argc != 3) {
        printf("Usage: ImageIn.ppm ImageOut.ppm \n"); 
        exit (1) ;
    }
    
    // Definition des variables
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    OCTET *ImgIn, *Y, *Cb, *Cr;

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);


    // Lecture de l'image d'entrée     
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    
    // Allocation de l'image de sortie
    allocation_tableau(Y, OCTET, nTaille);
    allocation_tableau(Cb, OCTET, nTaille);
    allocation_tableau(Cr, OCTET, nTaille);

    for (int i = 0; i < nH; ++i) {
        for (int j = 0; j < nW; ++j) {
            int index = i*nW + j;

            Y[index] = 0.299 * ImgIn[index*3] + 0.587 * ImgIn[(index*3) + 1] + 0.114 * ImgIn[(index*3) + 2];
            Cb[index] = -0.1687 * ImgIn[index*3] - 0.3313 * ImgIn[(index*3) + 1] + 0.5 * ImgIn[(index*3) + 2] + 128;
            Cr[index] = 0.5 * ImgIn[index*3] - 0.4187 * ImgIn[(index*3) + 1] - 0.0813  *ImgIn[(index*3) + 2] + 128;
        }
    }


    // Enregistrement
    char *imgY = (char *)malloc(strlen(cNomImgEcrite) + 7);
    char *imCb  = (char *)malloc(strlen(cNomImgEcrite) + 8); 
    char *imgCr  = (char *)malloc(strlen(cNomImgEcrite) + 8);

    sprintf(imgY, "%s_Y.pgm", cNomImgEcrite);
    sprintf(imCb, "%s_Cb.pgm", cNomImgEcrite);
    sprintf(imgCr, "%s_Cr.pgm", cNomImgEcrite);
    
    ecrire_image_pgm(imgY, Y,  nH, nW);
    ecrire_image_pgm(imCb, Cb,  nH, nW);
    ecrire_image_pgm(imgCr, Cr,  nH, nW);

    free(ImgIn);

    free(Y);
    free(Cb);
    free(Cr);

    return 1;
}