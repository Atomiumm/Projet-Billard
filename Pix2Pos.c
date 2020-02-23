#include <stdio.h>
#include <stdlib.h>
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

struct vect {
	int x;
	int y;
};


//Functions declaration

struct color intToColor(int colorInteger); //Outputs a color structure based on an integer input

int getScore(int index /*Designate the top left corner of the square*/, int Rmin, int Rmax, int Gmin, int Gmax, int Bmin, int Bmax, int height, int width, int ballSize); //Calculates the score of a given ballSize*ballSize square for a given color range

//Main writing

int main(int argc, char **argv) {
	int Lmin=15,Lmax=85,Cmin=15,Cmax=65,redBallRmin=160,redBallRmax=255,redBallGmin=0,redBallGmax=160,redBallBmin=0,redBallBmax=160,yellowBallRmin=140,yellowBallRmax=255,yellowBallGmin=140,yellowBallGmax=255,yellowBallBmin=0,yellowBallBmax=175,whiteBallRmin=100,whiteBallRmax=255,whiteBallGmin=100,whiteBallGmax=255,whiteBallBmin=100,whiteBallBmax=255,bgRmin=39,bgRmax=62,bgGmin=91,bgGmax=116,bgBmin=202,bgBmax=255,ballDiameter=11;

	if (argc==30) {
		Lmin=atoi(argv[1]);
		Lmax=atoi(argv[2]);
		Cmin=atoi(argv[3]);
		Cmax=atoi(argv[4]);
		redBallRmin=atoi(argv[5]);
		redBallRmax=atoi(argv[6]);
		redBallGmin=atoi(argv[7]);
		redBallGmax=atoi(argv[8]);
		redBallBmin=atoi(argv[9]);
		redBallBmax=atoi(argv[10]);
		yellowBallRmin=atoi(argv[11]);
		yellowBallRmax=atoi(argv[12]);
		yellowBallGmin=atoi(argv[13]);
		yellowBallGmax=atoi(argv[14]);
		yellowBallBmin=atoi(argv[15]);
		yellowBallBmax=atoi(argv[16]);
		whiteBallRmin=atoi(argv[17]);
		whiteBallRmax=atoi(argv[18]);
		whiteBallGmin=atoi(argv[19]);
		whiteBallGmax=atoi(argv[20]);
		whiteBallBmin=atoi(argv[21]);
		whiteBallBmax=atoi(argv[22]);
		bgRmin=atoi(argv[23]);
		bgRmax=atoi(argv[24]);
		bgGmin=atoi(argv[25]);
		bgGmax=atoi(argv[26]);
		bgBmin=atoi(argv[27]);
		bgBmax=atoi(argv[28]);
		ballDiameter=atoi(argv[29]);
		if (0/*negative value or L,Cmin>max*/) {
			printf("Error : invalid values passed as a distance, cannot continue\n");
			return 0;
		}
		if (0/*color intensity>255 or <0; or min>max*/) {
			printf("Error : invalid color range, cannot continue\n");
			return 0;
		}
	} else {
		printf("invalid number of argument, continuing with default values\n");
	}

	//int widthTable = Lmax-Lmin+1;
	//int heightTable = Cmax-Cmin+1;
	int redScore=0,yellowScore=0,whiteScore=0, testScore=0;
	int pixelIndex=0;
	struct vect redPosition={0,0}, yellowPosition={0,0}, whitePosition={0,0};

	int i,j;
	for (i=Lmin;i<=Lmax-ballDiameter+1;i++) {
		for (j=Cmin;j<=Cmax-ballDiameter+1;j++) {
			pixelIndex=i+j*myW;
			testScore = getScore(pixelIndex,redBallRmin,redBallRmax,redBallGmin,redBallGmax,redBallBmin,redBallBmax,myH,myW,ballDiameter);
			if (testScore>redScore) {
				redScore=testScore;
				redPosition.x=i;
				redPosition.y=j;
			}
			testScore = getScore(pixelIndex,yellowBallRmin,yellowBallRmax,yellowBallGmin,yellowBallGmax,yellowBallBmin,yellowBallBmax,myH,myW,ballDiameter);
			if (testScore>yellowScore) {
				yellowScore=testScore;
				yellowPosition.x=i;
				yellowPosition.y=j;
			}
			testScore = getScore(pixelIndex,whiteBallRmin,whiteBallRmax,whiteBallGmin,whiteBallGmax,whiteBallBmin,whiteBallBmax,myH,myW,ballDiameter);
			if (testScore>whiteScore) {
				whiteScore=testScore;
				whitePosition.x=i;
				whitePosition.y=j;
			}
		}
	}

	printf("Score red : %d score yellow %d : score white : %d\n",getScore(5040,160,255,0,160,0,160,myH,myW,ballDiameter),getScore(4060,140,255,140,255,0,175,myH,myW,ballDiameter),getScore(3020,100,255,100,255,100,255,myH,myW,ballDiameter)); //print de test, calcul des score des vrais positions
	printf("Red: %d, %d, %d\nYellow: %d, %d, %d\nWhite: %d, %d, %d\n",redPosition.x,redPosition.y,redScore,yellowPosition.x,yellowPosition.y,yellowScore,whitePosition.x,whitePosition.y,whiteScore);
}

//Other functions writing

struct color intToColor(int colorInteger) {
	struct color colorOutput;
	colorOutput.blue = colorInteger%256;
	colorOutput.green = (colorInteger/256)%256;
	colorOutput.red = (colorInteger/65536)%256;
	return colorOutput;
}

int getScore(int index, int Rmin, int Rmax, int Gmin, int Gmax, int Bmin, int Bmax, int height, int width, int ballSize) {
	int score = 0;
	struct color pixColor={0,0,0};

	if ((index%myW>myW-ballSize)||(index/myW>myH-ballSize)) {
		printf("Error : square corner too close to image border");
		return -1;
	};

	for (int k=0;k<ballSize*ballSize;k++) {
		pixColor = intToColor(myPM[index+k*myW/ballSize+k%ballSize]);
		if ((pixColor.red>=Rmin)&&(pixColor.red<=Rmax)&&(pixColor.green>=Gmin)&&(pixColor.green<=Gmax)&&(pixColor.blue>=Bmin)&&(pixColor.blue<=Bmax)) score++;
	}

	return score;
}