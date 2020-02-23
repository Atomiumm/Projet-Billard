#include <stdio.h>
#include <string.h>
#include "PM.h" //provisoire

/*
Le programme va lire le fichier Pixmap.bin contenant les pixels de l’image. 
Les coordonnées des pixels des boules couleurs seront calculées et sauvées dans le fichier Pos.txt
*/

/*
Les ranges de couleurs des 3 boules ainsi que pour le fond bleu sont donnés
Les dimensions du pixmap sont données
Toutes valeurs du fichier sont encodée au format unsigned int
Format couleur 0x00RRGGBB
Les boules ne se superposent pas
Une boule fait 11x11 pixels
*/

/*
Pixmap.bin:
Largeur de l'image en pixels
Hauteur de l'image en pixels
pixel0
pixel1
.
.
last pixel
*/

/*
Pos.txt
Red: Xred, Yred, ScoreRed
Yellow: Xyellow, Yyellow, ScoreYellow
White: Xwhite, Ywhite, ScoreWhite
*/

/*Structures declaration*/

struct color {
	int red;
	int green;
	int blue;
};

//Functions declaration

struct color intToColor(int colorInteger); //Outputs a color structure based on an integer input

int getScore(int index /*Designate the top left corner of the square*/, int Rmin, int Rmax, int Gmin, int Gmax, int Bmin, int Bmax, int height, int width); //Calculates the score of a given 11*11 square for a given color range

/*Main writing*/

int main(int argc, char **argv) {
	struct color pixColor;
	pixColor = intToColor(myPM[0]);
	printf("test int to color : R%d G%d B%d",pixColor.red,pixColor.green,pixColor.blue);
    return 0;
}

//Other functions writing

struct color intToColor(int colorInteger) {
	struct color colorOutput;
	colorOutput.blue = colorInteger%256;
	colorOutput.green = (colorInteger/256)%256;
	colorOutput.red = (colorInteger/65536)%256;
	return colorOutput;
}

int getScore(int index, int Rmin, int Rmax, int Gmin, int Gmax, int Bmin, int Bmax, int height, int width) {
	int score = 0;
	int pixColor={0,0,0};

	if (index%11>width-11||index/11>height-11) return -1;

	for (int i=0;i<121;i++) {
		pixColor = intToColor(myPM[index+i*width/11+i%11]);
		if (pixColor.red>=Rmin&&pixColor.red<=Rmax&&pixColor.green>=Gmin&&pixColor.green<=Gmax&&pixColor.blue>=Bmin&&pixColor.blue<=Bmax) score++;
	}

	return score;
}