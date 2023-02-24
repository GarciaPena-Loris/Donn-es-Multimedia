// flou_multiple.cpp : floute plusieurs fois une image en niveau de gris
#include <stdio.h>
#include "image_ppm.h"
#include <string.h>

int main(int argc, char* argv[])
{
    system("./YCbCrBGR ../images/Monarch_Y.pgm ../images/Monarch_Cb.pgm ../images/Monarch_Cr.pgm ../newImages/Monarch_BGR.ppm");
    system("./YCbCrBRG ../images/Monarch_Y.pgm ../images/Monarch_Cb.pgm ../images/Monarch_Cr.pgm ../newImages/Monarch_BRG.ppm");
    system("./YCbCrGBR ../images/Monarch_Y.pgm ../images/Monarch_Cb.pgm ../images/Monarch_Cr.pgm ../newImages/Monarch_GBR.ppm");
    system("./YCbCrGRB ../images/Monarch_Y.pgm ../images/Monarch_Cb.pgm ../images/Monarch_Cr.pgm ../newImages/Monarch_GRB.ppm");
    system("./YCbCrRBG ../images/Monarch_Y.pgm ../images/Monarch_Cb.pgm ../images/Monarch_Cr.pgm ../newImages/Monarch_RBG.ppm");
    system("./YCbCrRGB ../images/Monarch_Y.pgm ../images/Monarch_Cb.pgm ../images/Monarch_Cr.pgm ../newImages/Monarch_RGB.ppm");

    return 1;
}
