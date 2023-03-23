#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;

  if (argc != 4)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm Seuil \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);
  sscanf(argv[3], "%d", &S);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

  for (int x = 0; x < nW; x++)
    for (int y = 0; y < nH; y++)
    {
      if (ImgIn[indicePixel(x, y, nW)] < S)
        ImgOut[indicePixel(x, y, nW)] = 0;
      else
        ImgOut[indicePixel(x, y, nW)] = 255;
    }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

  free(ImgIn);
  free(ImgOut);

  return 1;
}