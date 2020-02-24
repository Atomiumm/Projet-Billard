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

//Outputs red, green and blue components of an integer color code in a custom structures
struct color intToColor(int colorInteger); //Outputs a color structure based on an integer input

//Calculates the number of pixels in a certain color range (score) in a given ballSize*ballSize square
int getScore(int index /*Designate the top left corner of the square*/, struct color min, struct color max, int imageHeight, int imageWidth, int ballSize); 

//Main writing

int main(int argc, char **argv) {

	//Initializing with a default value the variables that will store the program arguments
	int ballDiameter=11;
	struct vect topLeftCorner={15,15}, bottomRightCorner={85,65};
	struct color redBallMin = {160,0,0}, redBallMax = {255,160,160}, yellowBallMin = {140,140,0}, yellowBallMax = {255,255,175}, whiteBallMin = {100,100,100}, whiteBallMax = {255,255,255}, backGroundMin = {39,62,91}, backGroundMax = {116,202,255};

	//Storing the program arguments while testing their validity
	if (argc==30) {
		topLeftCorner.x     = atoi(argv[1]);
		bottomRightCorner.x = atoi(argv[2]);
		topLeftCorner.y     = atoi(argv[3]);
		bottomRightCorner.y = atoi(argv[4]);
		redBallMin.red      = atoi(argv[5]);
		redBallMax.red      = atoi(argv[6]);
		redBallMin.green    = atoi(argv[7]);
		redBallMax.green    = atoi(argv[8]);
		redBallMin.blue     = atoi(argv[9]);
		redBallMax.blue     = atoi(argv[10]);
		yellowBallMin.red   = atoi(argv[11]);
		yellowBallMax.red   = atoi(argv[12]);
		yellowBallMin.green = atoi(argv[13]);
		yellowBallMax.green = atoi(argv[14]);
		yellowBallMin.blue  = atoi(argv[15]);
		yellowBallMax.blue  = atoi(argv[16]);
		whiteBallMin.red    = atoi(argv[17]);
		whiteBallMax.red    = atoi(argv[18]);
		whiteBallMin.green  = atoi(argv[19]);
		whiteBallMax.green  = atoi(argv[20]);
		whiteBallMin.blue   = atoi(argv[21]);
		whiteBallMax.blue   = atoi(argv[22]);
		backGroundMin.red   = atoi(argv[23]);
		backGroundMax.red   = atoi(argv[24]);
		backGroundMin.green = atoi(argv[25]);
		backGroundMax.green = atoi(argv[26]);
		backGroundMin.blue  = atoi(argv[27]);
		backGroundMax.blue  = atoi(argv[28]);
		ballDiameter        = atoi(argv[29]);

		int errFlag = 0;
		if (topLeftCorner.x > bottomRightCorner.x || topLeftCorner.y > bottomRightCorner.y || topLeftCorner.x < 0 || topLeftCorner.y < 0 || bottomRightCorner.x > myW || bottomRightCorner.y > myH) {
			printf("Error : invalid values passed as table size, cannot continue\n");
			errFlag = 1;
		}
		if (redBallMin.red > redBallMax.red || redBallMin.green > redBallMax.green || redBallMin.blue > redBallMax.blue || redBallMin.red < 0 || redBallMin.green < 0 || redBallMin.blue < 0 || redBallMax.red > 255 || redBallMax.green > 255 || redBallMax.blue > 255) {
			printf("Error : invalid values passed as red ball colour range, cannot continue\n");
			errFlag = 1;
		}
		if (yellowBallMin.red > yellowBallMax.red || yellowBallMin.green > yellowBallMax.green || yellowBallMin.blue > yellowBallMax.blue || yellowBallMin.red < 0 || yellowBallMin.green < 0 || yellowBallMin.blue < 0 || yellowBallMax.red > 255 || yellowBallMax.green > 255 || yellowBallMax.blue > 255) {
			printf("Error : invalid values passed as yellow ball colour range, cannot continue\n");
			errFlag = 1;
		}
		if (whiteBallMin.red > whiteBallMax.red || whiteBallMin.green > whiteBallMax.green || whiteBallMin.blue > whiteBallMax.blue || whiteBallMin.red < 0 || whiteBallMin.green < 0 || whiteBallMin.blue < 0 || whiteBallMax.red > 255 || whiteBallMax.green > 255 || whiteBallMax.blue > 255) {
			printf("Error : invalid values passed as white ball colour range, cannot continue\n");
			errFlag = 1;
		}
		if (backGroundMin.red > backGroundMax.red || backGroundMin.green > backGroundMax.green || backGroundMin.blue > backGroundMax.blue || backGroundMin.red < 0 || backGroundMin.green < 0 || backGroundMin.blue < 0 || backGroundMax.red > 255 || backGroundMax.green > 255 || backGroundMax.blue > 255) {
			printf("Error : invalid values passed as background colour range, cannot continue\n");
			errFlag = 1;
		}
		if (ballDiameter > bottomRightCorner.x-topLeftCorner.x || ballDiameter > bottomRightCorner.y-topLeftCorner.y) {
			printf("Error : ball is larger than table, cannot process\n");
			errFlag = 1;
		}

		if (errFlag) return 0;

		} else {
			printf("invalid number of argument, continuing with default values\n");
		}

	//Once program arguments registered, initializing other useful variables
	int redScore=0,yellowScore=0,whiteScore=0, testScore=0;
	int pixelIndex=0;
	struct vect redPosition={0,0}, yellowPosition={0,0}, whitePosition={0,0};

	//Scans the image within the limits of the billiard table to spot the highest score earning areas
	for (int xPos=topLeftCorner.x;xPos<=bottomRightCorner.x-ballDiameter+1;xPos++) {
		for (int yPos=topLeftCorner.y;yPos<=bottomRightCorner.y-ballDiameter+1;yPos++) {
			pixelIndex = xPos + yPos*myW;
			testScore = getScore(pixelIndex,redBallMin,redBallMax,myH,myW,ballDiameter);
			if (testScore > redScore) {
				redScore      = testScore;
				redPosition.x = xPos;
				redPosition.y = yPos;
			}
			testScore = getScore(pixelIndex,yellowBallMin,yellowBallMax,myH,myW,ballDiameter);
			if (testScore > yellowScore) {
				yellowScore      = testScore;
				yellowPosition.x = xPos;
				yellowPosition.y = yPos;
			}
			testScore = getScore(pixelIndex,whiteBallMin,whiteBallMax,myH,myW,ballDiameter);
			if (testScore > whiteScore) {
				whiteScore      = testScore;
				whitePosition.x = xPos;
				whitePosition.y = yPos;
			}
		}
	}

	printf("Red: %d, %d, %d\nYellow: %d, %d, %d\nWhite: %d, %d, %d\n",redPosition.x,redPosition.y,redScore,yellowPosition.x,yellowPosition.y,yellowScore,whitePosition.x,whitePosition.y,whiteScore);
}

//Other functions writing

struct color intToColor(int colorInteger) {
	struct color colorOutput;
	colorOutput.blue  = colorInteger % 256;
	colorOutput.green = (colorInteger/256) % 256;
	colorOutput.red   = (colorInteger/65536) % 256;
	return colorOutput;
}

int getScore(int index, struct color minRange, struct color maxRange, int imageHeight, int imageWidth, int ballDiameter) {

	int score = 0;
	struct color pixColor={0,0,0};

	//Verifying that all the area we will check for is within the limits of the image
	if (index%imageWidth > imageWidth-ballDiameter || index/imageWidth > imageHeight-ballDiameter) {
		printf("Error : trying to get score too close to image limits, trying to access pixels outside image");
		return -1;
	};

	//Scanning the area of interest for pixels within the range, and incrementing the score for each valid pixel
	for (int xPos = index % imageWidth ; xPos < index%imageWidth + ballDiameter ; xPos++) {
		for (int yPos = index/imageWidth ; yPos < index/imageWidth + ballDiameter ; yPos++) {
			pixColor = intToColor(myPM[xPos + yPos*imageWidth]);
			if (pixColor.red >= minRange.red && pixColor.red <= maxRange.red && pixColor.green >= minRange.green && pixColor.green <= maxRange.green && pixColor.blue >= minRange.blue && pixColor.blue <= maxRange.blue) score++;
		}
	}

	return score;
}