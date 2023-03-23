#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

  for (int y = 0; y < nH; y++)
    for (int x = 0; x < nW; x++)
    {
      if (y == 0 || x == 0 || y == nH - 1 || x == nW - 1)
      {
        ImgOut[indicePixel(x, y, nW)] = ImgIn[indicePixel(x, y, nW)];
      }
      else
      {
        ImgOut[indicePixel(x, y, nW)] =
            (ImgIn[indicePixel(x, y, nW)] +
             ImgIn[indiceVoisinHaut(x, y, nW)] +
             ImgIn[indiceVoisinBas(x, y, nW)] +
             ImgIn[indiceVoisinGauche(x, y, nW)] +
             ImgIn[indiceVoisinDroite(x, y, nW)] +
             ImgIn[indiceVoisinHautGauche(x, y, nW)] +
             ImgIn[indiceVoisinHautDroite(x, y, nW)] +
             ImgIn[indiceVoisinBasGauche(x, y, nW)] +
             ImgIn[indiceVoisinBasDroite(x, y, nW)]) /
            9;
      }
    }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

  free(ImgIn);
  free(ImgOut);

  return 1;
}
