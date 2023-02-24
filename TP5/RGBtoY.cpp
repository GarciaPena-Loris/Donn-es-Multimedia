#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char **argv) {
    // Gestion des arguments : on veut une image d'entrée et une image de sortie.
    
    if (argc != 3) 
        {
        printf("Usage: ImageIn.ppm ImageOut.pgm \n"); 
        exit (1) ;
        }
    
    // Deifinition des variables
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, nR, nG, nB;
    OCTET *ImgIn, *ImgOut;

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);


    // Lecture de l'image d'entrée     
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    
    // Allocation de l'image de sortie
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Passage en pgm
    for (int i=0; i < nTaille3; i+=3) {
        ImgOut[i/3] = (ImgIn[i] + ImgIn[i + 1] + ImgIn[i + 2]) / 3;
    }


    // Enregistrement
    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn);
    return 1;
  return 1;
}