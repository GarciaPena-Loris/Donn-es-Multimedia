// moyenneur.cpp

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    // init varaible
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;

    // verif args
    if (argc != 3)
    {
        printf("Usage: ImageIn.pgm ImageOut.pgm \n");
        exit(1);
    }

    // get args
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    // init taille img
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    // init imgs
    OCTET *ImgIn, *ImgOut;

    // init tabs
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // read imgs
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    /*================================main-code==================================*/

    int noyau_moyenneur[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}};

    filtre_3_3_pgm(ImgIn, ImgOut, nW, nH, noyau_moyenneur);

    /*===========================================================================*/

    // write imgs
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

    // free tabs
    free(ImgIn);
    free(ImgOut);

    // exit
    return 1;
}