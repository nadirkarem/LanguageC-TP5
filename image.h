#ifndef IMAGE_H
#define IMAGE_H

// Structure pour représenter une image
typedef struct {
    int **pixels;
    int largeur;
    int longueur;
    int valeur_max;
} Image;

// Déclaration des fonctions
Image loadImage(const char *filename);
void freeImage(Image *img);
void recordImage(const char *filename, const Image *img);
void binarisation(Image *img, int seuil);
void inverse(Image *img);
void erosion(Image *img, Image *img2);
void dilatation(Image *img, Image *img2);

#endif // IMAGE_H
