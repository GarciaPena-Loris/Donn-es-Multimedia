// ----------------------------------------------------------------------------
// Filename        : image_ppm.h
// Description     :
// Created On      : Tue Mar 31 13:26:36 2005
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define allocation_tableau(nom, type, nombre)                                   \
   if ((nom = (type *)calloc(nombre, sizeof(type))) == NULL)                    \
   {                                                                            \
      printf("\n Allocation dynamique impossible pour un pointeur-tableau \n"); \
      exit(1);                                                                  \
   }

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

typedef unsigned char OCTET;

/*===========================================================================*/
inline void ignorer_commentaires(FILE *f)
{
   unsigned char c;
   while ((c = fgetc(f)) == '#')
      while ((c = fgetc(f)) != '\n')
         ;
   fseek(f, -sizeof(unsigned char), SEEK_CUR);
}
/*===========================================================================*/

/*===========================================================================*/
inline void ecrire_image_ppm(char nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
   FILE *f_image;
   int taille_image = 3 * nb_colonnes * nb_lignes;

   if ((f_image = fopen(nom_image, "wb")) == NULL)
   {
      printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
      exit(EXIT_FAILURE);
   }
   else
   {
      fprintf(f_image, "P6\r"); /*ecriture entete*/
      fprintf(f_image, "%d %d\r255\r", nb_colonnes, nb_lignes);

      if ((fwrite((OCTET *)pt_image, sizeof(OCTET), taille_image, f_image)) != (size_t)(taille_image))
      {
         printf("\nErreur d'ecriture de l'image %s \n", nom_image);
         exit(EXIT_FAILURE);
      }
      fclose(f_image);
   }
}
/*===========================================================================*/

/*===========================================================================*/
inline void lire_nb_lignes_colonnes_image_ppm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
   FILE *f_image;
   int max_grey_val;

   /* cf : l'entete d'une image .ppm : P6                   */
   /*				       nb_colonnes nb_lignes */
   /*    			       max_grey_val          */

   if ((f_image = fopen(nom_image, "rb")) == NULL)
   {
      printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
      exit(EXIT_FAILURE);
   }
   else
   {
      fscanf(f_image, "P6 ");
      ignorer_commentaires(f_image);
      fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
      fclose(f_image);
   }
}
/*===========================================================================*/
/*===========================================================================*/
inline void lire_image_ppm(char nom_image[], OCTET *pt_image, int taille_image)
{
   FILE *f_image;
   int nb_colonnes, nb_lignes, max_grey_val;
   taille_image = 3 * taille_image;

   if ((f_image = fopen(nom_image, "rb")) == NULL)
   {
      printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
      exit(EXIT_FAILURE);
   }
   else
   {
      fscanf(f_image, "P6 ");
      ignorer_commentaires(f_image);
      fscanf(f_image, "%d %d %d%*c",
             &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

      if ((fread((OCTET *)pt_image, sizeof(OCTET), taille_image, f_image)) != (size_t)(taille_image))
      {
         printf("\nErreur de lecture de l'image %s \n", nom_image);
         exit(EXIT_FAILURE);
      }
      fclose(f_image);
   }
}

/*===========================================================================*/
/*===========================================================================*/

inline void planR(OCTET *pt_image, OCTET *src, int taille_image)
{
   int y;
   for (y = 0; y < taille_image; y++)
   {
      pt_image[y] = src[3 * y];
   }
}

/*===========================================================================*/
/*===========================================================================*/

inline void planV(OCTET *pt_image, OCTET *src, int taille_image)
{
   int y;
   for (y = 0; y < taille_image; y++)
   {
      pt_image[y] = src[3 * y + 1];
   }
}

/*===========================================================================*/
/*===========================================================================*/

inline void planB(OCTET *pt_image, OCTET *src, int taille_image)
{
   int y;
   for (y = 0; y < taille_image; y++)
   {
      pt_image[y] = src[3 * y + 2];
   }
}

/*===========================================================================*/
/*===========================================================================*/

inline void ecrire_image_pgm(char nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
   FILE *f_image;
   int taille_image = nb_colonnes * nb_lignes;

   if ((f_image = fopen(nom_image, "wb")) == NULL)
   {
      printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
      exit(EXIT_FAILURE);
   }
   else
   {
      fprintf(f_image, "P5\r"); /*ecriture entete*/
      fprintf(f_image, "%d %d\r255\r", nb_colonnes, nb_lignes);

      if ((fwrite((OCTET *)pt_image, sizeof(OCTET), taille_image, f_image)) != (size_t)taille_image)
      {
         printf("\nErreur de lecture de l'image %s \n", nom_image);
         exit(EXIT_FAILURE);
      }
      fclose(f_image);
   }
}

/*===========================================================================*/

inline void lire_nb_lignes_colonnes_image_pgm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
   FILE *f_image;
   int max_grey_val;

   /* cf : l'entete d'une image .pgm : P5                    */
   /*				       nb_colonnes nb_lignes */
   /*    			       max_grey_val          */

   if ((f_image = fopen(nom_image, "rb")) == NULL)
   {
      printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
      exit(EXIT_FAILURE);
   }
   else
   {
      fscanf(f_image, "P5 ");
      ignorer_commentaires(f_image);
      fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
      fclose(f_image);
   }
}
/*===========================================================================*/
/*===========================================================================*/
inline void lire_image_pgm(char nom_image[], OCTET *pt_image, int taille_image)
{
   FILE *f_image;
   int nb_colonnes, nb_lignes, max_grey_val;

   if ((f_image = fopen(nom_image, "rb")) == NULL)
   {
      printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
      exit(EXIT_FAILURE);
   }
   else
   {
      fscanf(f_image, "P5 ");
      ignorer_commentaires(f_image);
      fscanf(f_image, "%d %d %d%*c",
             &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

      if ((fread((OCTET *)pt_image, sizeof(OCTET), taille_image, f_image)) != (size_t)taille_image)
      {
         printf("\nErreur de lecture de l'image %s \n", nom_image);
         exit(EXIT_FAILURE);
      }
      fclose(f_image);
   }
}
/*===========================================================================*/
/*===========================================================================*/

inline int indicePixel(int x, int y, int width)
{
   return y * width + x;
}

/*====================================Voisins================================*/

inline int indiceVoisinGauche(int x, int y, int width)
{
   return y * width + (x - 1);
}

inline int indiceVoisinDroite(int x, int y, int width)
{
   return y * width + (x + 1);
}

inline int indiceVoisinHaut(int x, int y, int width)
{
   return (y - 1) * width + x;
}

inline int indiceVoisinBas(int x, int y, int width)
{
   return (y + 1) * width + x;
}

inline int indiceVoisinHautGauche(int x, int y, int width)
{
   return (y - 1) * width + (x - 1);
}

inline int indiceVoisinHautDroite(int x, int y, int width)
{
   return (y - 1) * width + (x + 1);
}

inline int indiceVoisinBasGauche(int x, int y, int width)
{
   return (y + 1) * width + (x - 1);
}

inline int indiceVoisinBasDroite(int x, int y, int width)
{
   return (y + 1) * width + (x + 1);
}

/*===========================================================================*/
/*===========================================================================*/

inline int indicePixel(int x, int y, int width, int composante)
{
   return 3 * (y * width + x) + composante;
}

/*====================================Voisins================================*/

inline int indiceVoisinGauche(int x, int y, int width, int composante)
{
   return 3 * (y * width + (x - 1)) + composante;
}

inline int indiceVoisinDroite(int x, int y, int width, int composante)
{
   return 3 * (y * width + (x + 1)) + composante;
}

inline int indiceVoisinHaut(int x, int y, int width, int composante)
{
   return 3 * ((y - 1) * width + x) + composante;
}

inline int indiceVoisinBas(int x, int y, int width, int composante)
{
   return 3 * ((y + 1) * width + x) + composante;
}

inline int indiceVoisinHautGauche(int x, int y, int width, int composante)
{
   return 3 * ((y - 1) * width + (x - 1)) + composante;
}

inline int indiceVoisinHautDroite(int x, int y, int width, int composante)
{
   return 3 * ((y - 1) * width + (x + 1)) + composante;
}

inline int indiceVoisinBasGauche(int x, int y, int width, int composante)
{
   return 3 * ((y + 1) * width + (x - 1)) + composante;
}

inline int indiceVoisinBasDroite(int x, int y, int width, int composante)
{
   return 3 * ((y + 1) * width + (x + 1)) + composante;
}

/*===========================================================================*/
/*===========================================================================*/

int pixelNoirVoisinage(OCTET *Img, int x, int y, int nW, int nH)
{
   for (int dx = -1; dx <= 1; dx++)
   {
      for (int dy = -1; dy <= 1; dy++)
      {
         int new_x = x + dx;
         int new_y = y + dy;
         if (new_x >= 0 && new_x < nW && new_y >= 0 && new_y < nH)
         {
            if (Img[indicePixel(new_x, new_y, nW)] == 0)
            {
               return 1;
            }
         }
      }
   }
   return 0;
}

int pixelBlancVoisinage(OCTET *Img, int x, int y, int nW, int nH)
{
   for (int dx = -1; dx <= 1; dx++)
   {
      for (int dy = -1; dy <= 1; dy++)
      {
         int new_x = x + dx;
         int new_y = y + dy;
         if (new_x >= 0 && new_x < nW && new_y >= 0 && new_y < nH)
         {
            if (Img[indicePixel(new_x, new_y, nW)] == 255)
            {
               return 1;
            }
         }
      }
   }
   return 0;
}

/*===========================================================================*/
/*===========================================================================*/

void filtre_3_3_pgm(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, int noyau[3][3])
{

   for (int x = 0; x < nW; x++)
   {
      for (int y = 0; y < nH; y++)
      {
         int val_voisins = 0;
         int coef_voisins = 0;

         // verif bord
         for (int voisin_x = max(0, x - 1); voisin_x <= min(x + 1, nW - 1); voisin_x++)
         {
            for (int voisin_y = max(0, y - 1); voisin_y <= min(y + 1, nH - 1); voisin_y++)
            {
               int coef = noyau[voisin_x - x + 1][voisin_y - y + 1];
               val_voisins += coef * ImgIn[(voisin_y * nW + voisin_x)];
               coef_voisins += coef;
            }
         }

         ImgOut[indicePixel(x,y,nW)] = val_voisins / coef_voisins;
      }
   }
}

/*===========================================================================*/
/*===========================================================================*/
