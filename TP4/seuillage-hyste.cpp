#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, SH, SB;

  if (argc != 5)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm Seuil-haut Seuil-bas \n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);
  sscanf(argv[3], "%d", &SH);
  sscanf(argv[4], "%d", &SB);

  OCTET *ImgIn, *ImgTamp, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

  allocation_tableau(ImgTamp, OCTET, nTaille);

  // premier seuillage Si Inorme(i, j) ≤ SB, alors 0. Si Inorme(i, j) ≥ SH , alors 255
  for (int x = 0; x < nW; x++)
  {
    for (int y = 0; y < nH; y++)
    {
      if (ImgIn[indicePixel(x, y, nW)] >= SH)
        ImgTamp[indicePixel(x, y, nW)] = 255;
      else if (ImgIn[indicePixel(x, y, nW)] < SB)
        ImgTamp[indicePixel(x, y, nW)] = 0;
      else
        ImgTamp[indicePixel(x, y, nW)] = ImgIn[indicePixel(x, y, nW)];
    }
  }

  // deuxième seuillage si SB < Inorme < SHet qu’au moins un de ses voisins est 255, alors 255. Sinon, 0
  for (int x = 0; x < nW; x++)
  {
    for (int y = 0; y < nH; y++)
    {
      if (ImgTamp[indicePixel(x, y, nW)] > SB && ImgTamp[indicePixel(x, y, nW)] < SH && pixelBlancVoisinage(ImgTamp, x, y, nW, nH))
        ImgOut[indicePixel(x, y, nW)] = 255;
      else
        ImgOut[indicePixel(x, y, nW)] = 0;
    }
  }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

  free(ImgIn);
  free(ImgTamp);
  free(ImgOut);

  return 1;
}