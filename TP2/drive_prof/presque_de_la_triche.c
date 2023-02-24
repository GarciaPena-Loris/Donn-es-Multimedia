/*
Affichage du profil d'une ligne/colonne sur une image Grayscale
*/
int numero_de_ligne_ou_colonne = 128; // Ligne/colonne a afficher
bool on_s_occupe_d_une_ligne = true; // Est-ce qu'on affiche le profil d'une ligne ou alors d'une colonne ?
int* profil;	// Va contenir les valeurs des pixels
int nb_element_du_profil; // Quelle taille va faire notre profil? (largeur ou hauteur de l'image)

if (on_s_occupe_d_une_ligne) {	// Si on affiche une ligne  
	nb_element_du_profil = nW;	// le profil affiche nW pixels (largeur)
} else {
	nb_element_du_profil = nH;	// Sinon c'est nH pixels (hauteur)
}
// On alloue la taille du tableau qu'il nous faut
// On pourrait le remplacer par "allocation_tableau(profil, int, nb_element_du_profil);"
profil = new int[nb_element_du_profil]; // 1 case par pixel de la ligne
for (int i = 0; i < nb_element_du_profil; i++)
	profil[i] = 0;
// Pour ceux qui connaissent plus le C++, on aurait très bien pu utiliser un std::vector

if (on_s_occupe_d_une_ligne) {
	// Si on s'occuper d'une ligne, on ajoute chaque pixel de la ligne dans le profil
	for (int x = 0; x < nW; x++) {
		profil[i] = ImgIn[indicePixel(x, numero_de_ligne_ou_colonne, nW, nH)];
	}
} else {
	// Si on s'occuper d'une colonne, on ajoute chaque pixel de la colonne dans le profil
	for (int y = 0; y < nH; y++) {
		profil[i] = ImgIn[indicePixel(numero_de_ligne_ou_colonne, y, nW, nH)];
	}
}

// On affiche le tout dans la console
for (int i = 0; i < nb_element_du_profil; i++) {
	printf("%d %d", i, profil[i]);
}



/*
Affichage du profil d'une ligne/colonne sur une image RGB
*/

int numero_de_ligne_ou_colonne = 128; // Ligne/colonne a afficher
bool on_s_occupe_d_une_ligne = true; // Est-ce qu'on affiche le profil d'une ligne ou alors d'une colonne ?
int* profilR;	// Va contenir les valeurs des pixels rouges
int* profilG;	// Va contenir les valeurs des pixels verts
int* profilB;	// Va contenir les valeurs des pixels bleus
int nb_element_du_profil; // Quelle taille va faire notre profil? (largeur ou hauteur de l'image)

if (on_s_occupe_d_une_ligne) {	// Si on affiche une ligne  
	nb_element_du_profil = nW;	// le profil affiche nW pixels (largeur)
} else {
	nb_element_du_profil = nH;	// Sinon c'est nH pixels (hauteur)
}
// On alloue la taille du tableau qu'il nous faut
// On pourrait le remplacer par "allocation_tableau(profil, int, nb_element_du_profil);"
profilR = new int[nb_element_du_profil]; // 1 case par pixel de la ligne
profilG = new int[nb_element_du_profil]; // 1 case par pixel de la ligne
profilB = new int[nb_element_du_profil]; // 1 case par pixel de la ligne
for (int i = 0; i < nb_element_du_profil; i++) {
	profilR[i] = 0;
	profilG[i] = 0;
	profilB[i] = 0;
}
// Pour ceux qui connaissent plus le C++, on aurait très bien pu utiliser un std::vector

if (on_s_occupe_d_une_ligne) {
	// Si on s'occuper d'une ligne, on ajoute chaque pixel de la ligne dans le profil
	for (int x = 0; x < nW; x++) {
		profilR[i] = ImgIn[indicePixel(x, numero_de_ligne_ou_colonne, nW, nH, 0)]; // Indice de la composante rouge du pixel
		profilG[i] = ImgIn[indicePixel(x, numero_de_ligne_ou_colonne, nW, nH, 1)]; // Indice de la composante verte du pixel
		profilB[i] = ImgIn[indicePixel(x, numero_de_ligne_ou_colonne, nW, nH, 2)]; // Indice de la composante bleue du pixel
	}
} else {
	// Si on s'occuper d'une colonne, on ajoute chaque pixel de la colonne dans le profil
	for (int y = 0; y < nH; y++) {
		profilR[i] = ImgIn[indicePixel(numero_de_ligne_ou_colonne, y, nW, nH, 0)]; // Indice de la composante rouge du pixel
		profilG[i] = ImgIn[indicePixel(numero_de_ligne_ou_colonne, y, nW, nH, 1)]; // Indice de la composante verte du pixel
		profilB[i] = ImgIn[indicePixel(numero_de_ligne_ou_colonne, y, nW, nH, 2)]; // Indice de la composante bleue du pixel
	}
}

// On affiche le tout dans la console
for (int i = 0; i < nb_element_du_profil; i++) {
	printf("%d %d %d %d", i, profilR[i], profilG[i], profilB[i]);
}



/*
Application d'un traitement sur une image RGB : solution #1, la décomposition des couches
*/
OCTET *imgRouge, *imgVert, *imgBleu; // On crée 3 "petites" images qui vont contenir la composante rouge, verte, bleue
allocation_tableau(imgRouge, OCTET, nTaille); allocation_tableau(imgVert, OCTET, nTaille);  allocation_tableau(imgBleu, OCTET, nTaille);
planR(imgRouge, ImgIn, nTaille);  planV(imgVert, ImgIn, nTaille); planB(imgBleu, ImgIn, nTaille); // Séparation de l'image RGB avec des fonctions définies dans image_ppm.h 

// Utilisation des imgRouge, imgVert et imgBleu comme n'importe quelle image en niveau de gris

// Remettre chaque composante rouge, verte, bleue dans une image en couleur, pour l'enregistrer
for (int i = 0; i < nTaille3; i += 3) {    
    ImgOut[i] = imgRouge[i/3];
    ImgOut[i+1] = imgVert[i/3];
    ImgOut[i+2] = imgBleu[i/3];

}


/*
Application d'un traitement sur une image RGB : solution #2, la classique
Exemple du flou en croix
*/
// Version brute :
for (int x = 0; x < nW; x++) {
	for (int y = 0; y < nH; y++) {
		ImgOut[indicePixel(x, y, nW, nH, 0)] = (ImgIn[indicePixel(x - 1, y, nW, nH, 0)] + ImgIn[indicePixel(x, y, nW, nH, 0)] + ImgIn[indicePixel(x + 1, y, nW, nH, 0)] + ImgIn[indicePixel(x, y - 1, nW, nH, 0)] + ImgIn[indicePixel(x, y + 1, nW, nH, 0)]) / 5.0;
		ImgOut[indicePixel(x, y, nW, nH, 1)] = (ImgIn[indicePixel(x - 1, y, nW, nH, 1)] + ImgIn[indicePixel(x, y, nW, nH, 1)] + ImgIn[indicePixel(x + 1, y, nW, nH, 1)] + ImgIn[indicePixel(x, y - 1, nW, nH, 1)] + ImgIn[indicePixel(x, y + 1, nW, nH, 1)]) / 5.0;
		ImgOut[indicePixel(x, y, nW, nH, 2)] = (ImgIn[indicePixel(x - 1, y, nW, nH, 2)] + ImgIn[indicePixel(x, y, nW, nH, 2)] + ImgIn[indicePixel(x + 1, y, nW, nH, 2)] + ImgIn[indicePixel(x, y - 1, nW, nH, 2)] + ImgIn[indicePixel(x, y + 1, nW, nH, 2)]) / 5.0;
	}
}

// Version plus propre :
for (int x = 0; x < nW; x++) {
	for (int y = 0; y < nH; y++) {
		for (int c = 0; c < 3; c++) {
			ImgOut[indicePixel(x, y, nW, nH, c)] = (ImgIn[indicePixel(x - 1, y, nW, nH, c)] + ImgIn[indicePixel(x, y, nW, nH, c)] + ImgIn[indicePixel(x + 1, y, nW, nH, c)] + ImgIn[indicePixel(x, y - 1, nW, nH, c)] + ImgIn[indicePixel(x, y + 1, nW, nH, c)]) / 5.0;
		}
	}
}

/*
Calcul de l'indice d'un pixel sur une image Grayscale :
indicePixel(x, y, width, height) = y * width + x

Calcul de l'indice d'un pixel sur une image RGB :
indicePixel(x, y, width, height, composante) = 3 * (y * width + x) + composante
*/


/*
Affichage avec GNUPLOT :
$ gnuplot
> plot 'monHisto.dat' with lines
> replot 'monDeuxiemeHisto.dat' with lines 

Affichage de plusieurs colonnes d'un fichier :
$ gnuplot 
> plot 'monHistoRGB.dat' using 1:2 with lines lt rgb "red" title "Rouge"
> replot 'monHistoRGB.dat' using 1:3 with lines lt rgb "green" title "Vert"
> replot 'monHistoRGB.dat' using 1:4 with lines lt rgb "blue" title "Bleu"
*/