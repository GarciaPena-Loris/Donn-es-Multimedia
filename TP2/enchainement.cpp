// fermeture.cpp :

#include <stdio.h>
#include "image_ppm.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250], commande[500];

    if (argc != 3)
    {
        printf("Usage: chemin/vers/ImageIn.pgm ImageOut.pgm \n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    printf("Demarrage de 3 dilatation\n");
    // 3 dilatation
    sprintf(commande, "dilat.exe %s ../newImages/enchainement/imageEnchaineeTampon1.pgm", cNomImgLue);
    system(commande);
    sleep(0.05);
    system("dilat.exe ../newImages/enchainement/imageEnchaineeTampon1.pgm ../newImages/enchainement/imageEnchaineeTampon2.pgm");
    sleep(0.05);
    system("dilat.exe ../newImages/enchainement/imageEnchaineeTampon2.pgm ../newImages/enchainement/imageEnchaineeTampon3.pgm");
    sleep(0.05);
    printf("Dilatation termine\n");

    printf("Demarrage de 6 erosion\n");
    // 6 erosion
    system("ero.exe ../newImages/enchainement/imageEnchaineeTampon3.pgm ../newImages/enchainement/imageEnchaineeTampon4.pgm");
    sleep(0.05);
    system("ero.exe ../newImages/enchainement/imageEnchaineeTampon4.pgm ../newImages/enchainement/imageEnchaineeTampon5.pgm");
    sleep(0.05);
    system("ero.exe ../newImages/enchainement/imageEnchaineeTampon5.pgm ../newImages/enchainement/imageEnchaineeTampon6.pgm");
    sleep(0.05);
    system("ero.exe ../newImages/enchainement/imageEnchaineeTampon6.pgm ../newImages/enchainement/imageEnchaineeTampon7.pgm");
    sleep(0.05);
    system("ero.exe ../newImages/enchainement/imageEnchaineeTampon7.pgm ../newImages/enchainement/imageEnchaineeTampon8.pgm");
    sleep(0.05);
    system("ero.exe ../newImages/enchainement/imageEnchaineeTampon8.pgm ../newImages/enchainement/imageEnchaineeTampon9.pgm");
    sleep(0.05);
    printf("Erosion termine\n");

    printf("Demarrage de 3 dilatation\n");
    // 3 dilatation
    system("dilat.exe ../newImages/enchainement/imageEnchaineeTampon9.pgm ../newImages/enchainement/imageEnchaineeTampon10.pgm");
    sleep(0.05);
    system("dilat.exe ../newImages/enchainement/imageEnchaineeTampon10.pgm ../newImages/enchainement/imageEnchaineeTampon11.pgm");
    sleep(0.05);
    sprintf(commande, "dilat.exe ../newImages/enchainement/imageEnchaineeTampon11.pgm ../newImages/enchainement/%s", cNomImgEcrite);
    sleep(0.05);
    system(commande);
    printf("Dilatation termine\n");
    printf("Fin.");

    return 1;
}
