#include <stdio.h>
#include <math.h>        
#include "image_ppm.h"

int main(int argc, char* argv[]){
    char cNomImgLue[250], cNomImgEcrite[250];
    int lignes, colonnes, nTaille, S;

    if (argc != 3) {
        printf("Usage: ImageIn.pgm NomDesImages\n"); 
        exit (1);
    }

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);

    OCTET *ImgIn, *gradH, *gradV, *norme;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &lignes, &colonnes);
    nTaille = lignes * colonnes;

    allocation_tableau(ImgIn, OCTET, nTaille);

    //allocations pour les images de sorties (normes et gradients)
    allocation_tableau(gradH, OCTET, nTaille);
    allocation_tableau(gradV, OCTET, nTaille);
    allocation_tableau(norme, OCTET, nTaille);

    //on lis l'imageIn
    lire_image_pgm(cNomImgLue, ImgIn, lignes * colonnes);

    for (int i = 0; i < lignes; ++i) {
        for (int j = 0; j < colonnes; ++j) {
            int pointeur = i*colonnes + j;
            // On fais le gradV
                //1--- si on est au bord : on garde le même pixel
            if (i == lignes - 1){
                gradV[pointeur]=ImgIn[pointeur];
            } else {
                //2---- Sinon on effectue la soustraction du pixel suivant et du pointeur 
                int suivant = (i+1)*colonnes+j;
                gradV[pointeur]=abs(ImgIn[suivant]-ImgIn[pointeur]);
            }
            // On fais le gradH
                //1--- si on est au bord : on garde le même pixel
            if (j == colonnes - 1){
                gradH[pointeur]=ImgIn[pointeur];
            } else {
                //2---- Sinon on effectue la soustraction du pixel suivant et du pointeur 
                int suivant = pointeur+1;
                gradH[pointeur]=abs(ImgIn[suivant]-ImgIn[pointeur]);
            }
            //On calcul la norme 
            norme[pointeur]=sqrt((pow(gradH[pointeur],2)+pow(gradV[pointeur],2)));
        }

    }
    //On creer les nom fichiers
    char *imgNorme = (char *)malloc(strlen(cNomImgLue) + 5); // File + .pgm
    char *gradientV = (char *)malloc(strlen(cNomImgLue) + 8); // File + _gv.pgm
    char *gradientH = (char *)malloc(strlen(cNomImgLue) + 8); // File + _gh.pgm

    sprintf(imgNorme, "%s.pgm", cNomImgEcrite);
    sprintf(gradientV, "%s_gv.pgm", cNomImgEcrite);
    sprintf(gradientH, "%s_gh.pgm", cNomImgEcrite);


    //On écrit les images 
    ecrire_image_pgm(imgNorme, norme, lignes, colonnes);
    ecrire_image_pgm(gradientV, gradV, lignes, colonnes);
    ecrire_image_pgm(gradientH, gradH, lignes, colonnes);

    //On libere le monde
    free(ImgIn); free(gradV); free(gradH); free(norme); free(imgNorme); free(gradientV); free(gradientH);
    return 0;

}