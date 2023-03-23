#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 3)
  {
    printf("Usage: ImageIn.ppm ImageOut.ppm \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgIn, OCTET, nTaille3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille3);

  for (int y = 0; y < nH; y++)
  {
    for (int x = 0; x < nW; x++)
    {
      if (y == 0 || x == 0 || y == nH - 1 || x == nW - 1)
      {
        ImgOut[indicePixel(x, y, nW, 0)] = ImgIn[indicePixel(x, y, nW, 0)];
        ImgOut[indicePixel(x, y, nW, 1)] = ImgIn[indicePixel(x, y, nW, 1)];
        ImgOut[indicePixel(x, y, nW, 2)] = ImgIn[indicePixel(x, y, nW, 2)];
      }
      else
      {
        ImgOut[indicePixel(x, y, nW, 0)] =
            (ImgIn[indiceVoisinHautGauche(x, y, nW, 0)] +
             ImgIn[indiceVoisinHaut(x, y, nW, 0)] +
             ImgIn[indiceVoisinHautDroite(x, y, nW, 0)] +
             ImgIn[indiceVoisinGauche(x, y, nW, 0)] +
             ImgIn[indicePixel(x, y, nW, 0)] +
             ImgIn[indiceVoisinDroite(x, y, nW, 0)] +
             ImgIn[indiceVoisinBasGauche(x, y, nW, 0)] +
             ImgIn[indiceVoisinBas(x, y, nW, 0)] +
             ImgIn[indiceVoisinBasDroite(x, y, nW, 0)]) /
            9;

        ImgOut[indicePixel(x, y, nW, 1)] =
            (ImgIn[indiceVoisinHautGauche(x, y, nW, 1)] +
             ImgIn[indiceVoisinHaut(x, y, nW, 1)] +
             ImgIn[indiceVoisinHautDroite(x, y, nW, 1)] +
             ImgIn[indiceVoisinGauche(x, y, nW, 1)] +
             ImgIn[indicePixel(x, y, nW, 1)] +
             ImgIn[indiceVoisinDroite(x, y, nW, 1)] +
             ImgIn[indiceVoisinBasGauche(x, y, nW, 1)] +
             ImgIn[indiceVoisinBas(x, y, nW, 1)] +
             ImgIn[indiceVoisinBasDroite(x, y, nW, 1)]) /
            9;

        ImgOut[indicePixel(x, y, nW, 2)] =
            (ImgIn[indiceVoisinHautGauche(x, y, nW, 2)] +
             ImgIn[indiceVoisinHaut(x, y, nW, 2)] +
             ImgIn[indiceVoisinHautDroite(x, y, nW, 2)] +
             ImgIn[indiceVoisinGauche(x, y, nW, 2)] +
             ImgIn[indicePixel(x, y, nW, 2)] +
             ImgIn[indiceVoisinDroite(x, y, nW, 2)] +
             ImgIn[indiceVoisinBasGauche(x, y, nW, 2)] +
             ImgIn[indiceVoisinBas(x, y, nW, 2)] +
             ImgIn[indiceVoisinBasDroite(x, y, nW, 2)]) /
            9;
      }
    }
  }

  ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(ImgOut);

  return 1;
}
