// image.c

#include "image.h"
#include <stdio.h>
#include <stdlib.h>



int main() {
    // Utilisation des fonctions définies dans image.c
    Image img_base = loadImage("lena.pgm");
    Image img = loadImage("lena.pgm");
    Image img2 = loadImage("lena.pgm");
    Image img3 = loadImage("lena.pgm");
    printf("Image de taille %dx%d\n", img.largeur, img.longueur);

    // Appel de la fonction recordImage pour enregistrer l'image
    recordImage("lena_copy_base.pgm", &img);

    // Appliquer la binarisation avec un seuil de 128
    binarisation(&img2, 128);

    // Appel de la fonction recordImage pour enregistrer l'image
    recordImage("lena_copy_binaire.pgm", &img2);

    // Appliquer l'inversion
    inverse(&img2);

    // Appel de la fonction recordImage pour enregistrer l'image
    recordImage("lena_copy_inverse.pgm", &img2);

    // Appliquer l'érosion
    erosion(&img_base, &img3);

    // Appel de la fonction recordImage pour enregistrer l'image
    recordImage("lena_copy_erosion.pgm", &img3);

    // Appliquer la dilatation
    dilatation(&img_base, &img3);

    // Appel de la fonction recordImage pour enregistrer l'image
    recordImage("lena_copy_dilatation.pgm", &img3);

    // Libération de la mémoire
    freeImage(&img);
    freeImage(&img2);
    freeImage(&img3);

    return 0;
}

Image loadImage(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    // Lecture du format de l'image
    char format [3];
    fscanf(file, "%2s", format);

    // Lecture de la largeur, hauteur et valeur maximale
    int largeur, longueur, valeur_max;
    fscanf(file, "%d %d %d", &largeur, &longueur, &valeur_max);

    // Allocation dynamique pour la matrice de pixels
    int **pixels = (int **)malloc(longueur * sizeof(int *));
    if (!pixels) {
        fprintf(stderr, "Allocation mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < longueur; i++) {
        pixels[i] = (int *)malloc(largeur * sizeof(int));
        if (!pixels[i]) {
            fprintf(stderr, "Allocation mémoire échouée.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Lecture des valeurs de l'image
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < longueur; j++) {
            fscanf(file, "%d", &pixels[i][j]);
        }
    }

    // Fermeture du fichier
    fclose(file);

    // Création de la structure Image
    Image img;
    img.pixels = pixels;
    img.largeur = largeur;
    img.longueur = longueur;
    img.valeur_max = valeur_max;

    return img;
}

void freeImage(Image *img) {
    for (int i = 0; i < img->longueur; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
}

void recordImage(const char *filename, const Image *img) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier pour l'enregistrement.\n");
        exit(EXIT_FAILURE);
    }

    // Écriture du format de l'image
    fprintf(file, "P2\n");

    // Écriture de la largeur, de la hauteur et de la valeur maximale
    fprintf(file, "%d %d\n%d\n", img->largeur, img->longueur, img->valeur_max);

    // Écriture des valeurs des pixels
    for (int i = 0; i < img->largeur; i++) {
        for (int j = 0; j < img->longueur; j++) {
            fprintf(file, "%d ", img->pixels[i][j]);
        }
        fprintf(file, "\n");
    }

    // Fermeture du fichier
    fclose(file);
}

void binarisation(Image *img, int seuil) {
    for (int i = 0; i < img->largeur; i++) {
        for (int j = 0; j < img->longueur; j++) {
            if (img->pixels[i][j] > seuil) {
                img->pixels[i][j] = 255; // Mettre à 255 si la valeur est supérieure au seuil
            } else {
                img->pixels[i][j] = 0; // Mettre à 0 si la valeur est inférieure ou égale au seuil
            }
        }
    }
}

void inverse(Image *img) {
    for (int i = 0; i < img->largeur; i++) {
        for (int j = 0; j < img->longueur; j++) {
            
            if (img->pixels[i][j] == 255) {
                img->pixels[i][j] = 0;
            }
            else if (img->pixels[i][j] == 0) {
                img->pixels[i][j] = 255;
            }
            else {
                fprintf(stderr, "Erreur : la valeur du pixel n'est pas 0 ou 255.\n");
                exit(EXIT_FAILURE);
            }
            
            
        }
    }
}

void erosion(Image *img, Image *img2) {
    int tab [4];

    // Parcours de l'image
    for (int i = 1; i < img->largeur - 1; i++) {
        for (int j = 1; j < img->longueur - 1; j++) {
            // Calcul du minimum entre le pixel courant et ses voisins
            int min = img->pixels[i][j];
            tab[0] = img->pixels[i-1][j];
            tab[1] = img->pixels[i+1][j];
            tab[2] = img->pixels[i][j-1];
            tab[3] = img->pixels[i][j+1];
            for (int k = 0; k < 4; k++) {
                if (tab[k] < min) {
                    min = tab[k];
                }
            }
            /* 2ème méthode  non finie pour les voisin d'ordre 5.
            for (int x = -2 ; x<3 ; x++) {
                for (int y = -2 ; y<3 ; y++) {
                    int min = img->pixels[i][j];
                    tab[x+2][y+2] = img->pixels[i+x][j+y];
                    if (tab[x+2][y+2] < min) {
                        min = tab[x+2][y+2];
                    }
                    
                }
            }
            */
            // Stockage du résultat dans le tableau temporaire
            img2->pixels[i][j] = min;
        }
    }

}

void dilatation(Image *img,Image *img2) {
    int tab [4];

    // Parcours de l'image
    for (int i = 1; i < img->largeur - 1; i++) {
        for (int j = 1; j < img->longueur - 1; j++) {
            // Calcul du minimum entre le pixel courant et ses voisins
            int max = img->pixels[i][j];
            tab[0] = img->pixels[i-1][j];
            tab[1] = img->pixels[i+1][j];
            tab[2] = img->pixels[i][j-1];
            tab[3] = img->pixels[i][j+1];
            for (int k = 0; k < 4; k++) {
                if (tab[k] > max) {
                    max = tab[k];
                }
            }

            // Stockage du résultat dans le tableau temporaire
            img2->pixels[i][j] = max;
        }
    }

}

