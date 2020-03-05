#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MIN_LEGAL_IMAGE_SIZE 10
#define MAX_LEGAL_IMAGE_SIZE 1000
#define MIN_LEGAL_BALL_SIZE 5
#define MAX_LEGAL_BALL_SIZE 20

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

//Structures declaration

struct color {
	int red;
	int green;
	int blue;
};

struct vect {
	int x;
	int y;
};

struct vectPlus {
	int x;
	int y;
	int score;
};


//Functions declaration

//Outputs red, green and blue components of an integer color code in a custom structures
struct color intToColor(int colorInteger); //Outputs a color structure based on an integer input

//Calculates the number of pixels in a certain color range (score) in a given ballSize*ballSize square
int getScore(unsigned int *pixMap, int index /*Designate the top left corner of the square*/, struct color minRange, struct color maxRange, int imageHeight, int imageWidth, int ballDiameter, char mode); 

struct vectPlus convergence(unsigned int *pixMap, int index, struct color minRange, struct color maxRange, int imageHeight, int imageWidth, int ballDiameter);

//Main writing

int main(int argc, char **argv) {

	time_t begin = clock();

	//Initializing with a default value the variables that will store the program arguments
	int ballDiameter=11;
	struct vect topLeftCorner={15,15}, bottomRightCorner={85,65};
	struct color redBallMin = {160,0,0}, redBallMax = {255,160,160}, yellowBallMin = {140,140,0}, yellowBallMax = {255,255,175}, whiteBallMin = {100,100,100}, whiteBallMax = {255,255,255}, backGroundMin = {39,62,91}, backGroundMax = {116,202,255};

	//An error flag tracking various errors. Allow to check and warn for different errors before exiting the program
	int errFlag = 0;
	//extracting data from input file
	unsigned int imageWidth;
	unsigned int imageHeight;
	unsigned int *pixMap;
	unsigned int storeReturn;

	FILE *binmap;
	binmap = fopen("Pixmap.bin","rb");
	if (binmap==NULL) {
		printf("Error : cannot access pixmap\n");
		errFlag = 1;
	}

	storeReturn = fread(&imageWidth,sizeof(unsigned int),1,binmap);
	if (storeReturn!=1) {
		printf("Error : cannot read image width, cannot continue\n");
		errFlag = 1;
	}
	if (imageWidth > MAX_LEGAL_IMAGE_SIZE || imageWidth < MIN_LEGAL_IMAGE_SIZE) {
		printf("Error : image width illegal, should be comprised in the range %d:%d\n",MIN_LEGAL_IMAGE_SIZE,MAX_LEGAL_IMAGE_SIZE);
		errFlag = 1;
	}

	storeReturn = fread(&imageHeight,sizeof(unsigned int),1,binmap);
	if (storeReturn!=1) {
		printf("Error : cannot read image height, cannot continue\n");
		errFlag = 1;
	}
	if (imageHeight > MAX_LEGAL_IMAGE_SIZE || imageHeight < MIN_LEGAL_IMAGE_SIZE) {
		printf("Error : image height illegal, should be comprised in the range %d:%d\n",MIN_LEGAL_IMAGE_SIZE,MAX_LEGAL_IMAGE_SIZE);
		errFlag = 1;
	}

	if (errFlag) {
		fclose(binmap);
		return -1; //end the program if an error as occured since errors in file reading prevents further advancement
	}

	pixMap = malloc(imageWidth*imageHeight*sizeof(unsigned int));
	if (pixMap==NULL) {
		printf("Error : cannot allocate space for the pixMap\n");
		free(pixMap);
		fclose(binmap);
		return -1; //instantly ends the program since this error will prevent any further advancement
	}

	storeReturn = fread(pixMap,sizeof(unsigned int),imageWidth*imageHeight,binmap);
	if (storeReturn!=imageWidth*imageHeight) {
		printf("Error : cannot read pixel values, file may not contain enough data\n");
		free(pixMap);
		fclose(binmap);
		return -1;
	}

	if (fread(&storeReturn,sizeof(unsigned int),1,binmap)==1) {
		printf("Warning : more pixels availables than anticipated, results, if any, are to be interpreted with caution\n");
	}

	fclose(binmap);

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

		if (topLeftCorner.x > bottomRightCorner.x || topLeftCorner.y > bottomRightCorner.y || topLeftCorner.x < 0 || topLeftCorner.y < 0 || bottomRightCorner.x > imageWidth || bottomRightCorner.y > imageHeight) {
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
		if (ballDiameter < MIN_LEGAL_BALL_SIZE || ballDiameter > MAX_LEGAL_BALL_SIZE) {
			printf("Error : ball diameter illegal, should be comprised in the range %d:%d\n",MIN_LEGAL_BALL_SIZE,MAX_LEGAL_BALL_SIZE);
			errFlag = 1;
		}
		if (ballDiameter > bottomRightCorner.x-topLeftCorner.x || ballDiameter > bottomRightCorner.y-topLeftCorner.y) {
			printf("Error : ball is larger than table, cannot process\n");
			errFlag = 1;
		}

	} else {
		printf("Error : invalid number of arguments\n");
		errFlag = 1;
	}

	if (errFlag) {
		free(pixMap);
		return -1;
	}

	int crossScore;
	int pixIndex;
	struct vectPlus redPosition, yellowPosition, whitePosition, positionUpdate;
	int highScoringRed=0, highScoringYellow=0, highScoringWhite=0;
	int rightRedBall=-1, leftRedBall=imageWidth, upRedBall=imageHeight, downRedBall=-1, rightYellowBall=-1, leftYellowBall=imageWidth, upYellowBall=imageHeight, downYellowBall=-1, rightWhiteBall=-1, leftWhiteBall=imageWidth, upWhiteBall=imageHeight, downWhiteBall=-1;

	int scoreThreshold = 3*ballDiameter*ballDiameter/4;

	int xPos = topLeftCorner.x + ballDiameter/2;
	while (xPos < bottomRightCorner.x) {
		int yPos = topLeftCorner.y + ballDiameter/2;
		while (yPos < bottomRightCorner.y) {

			pixIndex = xPos+yPos*imageWidth;
			crossScore = getScore(pixMap,pixIndex,redBallMin,redBallMax,imageHeight,imageWidth,ballDiameter,'c');
			if (crossScore!=0) {
				positionUpdate = convergence(pixMap,pixIndex,redBallMin,redBallMax,imageHeight,imageWidth,ballDiameter);
				if (positionUpdate.score > scoreThreshold) {
					redPosition = positionUpdate;
					highScoringRed++;
					if (redPosition.x > rightRedBall) rightRedBall=redPosition.x;
					if (redPosition.x < leftRedBall) leftRedBall=redPosition.x;
					if (redPosition.y > downRedBall) downRedBall=redPosition.y;
					if (redPosition.y < upRedBall) upRedBall=redPosition.y;
				}
			}

			crossScore = getScore(pixMap,pixIndex,yellowBallMin,yellowBallMax,imageHeight,imageWidth,ballDiameter,'c');
			if (crossScore!=0) {
				positionUpdate = convergence(pixMap,pixIndex,yellowBallMin,yellowBallMax,imageHeight,imageWidth,ballDiameter);
				if (positionUpdate.score > scoreThreshold) {
					yellowPosition = positionUpdate;
					highScoringYellow++;
					if (yellowPosition.x > rightYellowBall) rightYellowBall=yellowPosition.x;
					if (yellowPosition.x < leftYellowBall) leftYellowBall=yellowPosition.x;
					if (yellowPosition.y > downYellowBall) downYellowBall=yellowPosition.y;
					if (yellowPosition.y < upYellowBall) upYellowBall=yellowPosition.y;
				}
			}

			crossScore = getScore(pixMap,pixIndex,whiteBallMin,whiteBallMax,imageHeight,imageWidth,ballDiameter,'c');
			if (crossScore!=0) {
				positionUpdate = convergence(pixMap,pixIndex,whiteBallMin,whiteBallMax,imageHeight,imageWidth,ballDiameter);
				if (positionUpdate.score > scoreThreshold) {
					whitePosition = positionUpdate;
					highScoringWhite++;
					if (whitePosition.x > rightWhiteBall) rightWhiteBall=whitePosition.x;
					if (whitePosition.x < leftWhiteBall) leftWhiteBall=whitePosition.x;
					if (whitePosition.y > downWhiteBall) downWhiteBall=whitePosition.y;
					if (whitePosition.y < upWhiteBall) upWhiteBall=whitePosition.y;
				}
			}

			yPos += ballDiameter/2;
		}
		xPos += ballDiameter/2;
	}


	if (!highScoringRed) {
		printf("Error : Red ball is too small or nonexistant\n");
		errFlag = 1;
	}
	if (rightRedBall - leftRedBall > 2*ballDiameter|| downRedBall - upRedBall > 2*ballDiameter) {
		printf("Error : Multiple red balls\n");
		errFlag = 1;
	}
	if (!highScoringYellow) {
		printf("Error : Yellow ball is too small or nonexistant\n");
		errFlag = 1;
	}
	if (rightYellowBall - leftYellowBall > 2*ballDiameter|| downYellowBall - upYellowBall > 2*ballDiameter) {
		printf("Error : Multiple Yellow balls\n");
		errFlag = 1;
	}
	if (!highScoringWhite) {
		printf("Error : White ball is too small or nonexistant\n");
		errFlag = 1;
	}
	if (rightWhiteBall - leftWhiteBall > 2*ballDiameter|| downWhiteBall - upWhiteBall > 2*ballDiameter) {
		printf("Error : Multiple white balls\n");
		errFlag = 1;
	}

	if ((redPosition.x-yellowPosition.x)*(redPosition.x-yellowPosition.x) + (redPosition.y-yellowPosition.y)*(redPosition.y-yellowPosition.y) < ballDiameter*ballDiameter) {
		printf("Error : superposition of red and yellow ball\n");
		errFlag = 1;
	}
	if ((redPosition.x-whitePosition.x)*(redPosition.x-whitePosition.x) + (redPosition.y-whitePosition.y)*(redPosition.y-whitePosition.y) < ballDiameter*ballDiameter) {
		printf("Error : superposition of red and white ball\n");
		errFlag = 1;
	}
	if ((whitePosition.x-yellowPosition.x)*(whitePosition.x-yellowPosition.x) + (whitePosition.y-yellowPosition.y)*(whitePosition.y-yellowPosition.y) < ballDiameter*ballDiameter) {
		printf("white : %d %d ; yellow : %d %d\n",whitePosition.x,whitePosition.y,yellowPosition.x,yellowPosition.y);
		printf("Error : superposition of yellow and white ball\n");
		errFlag = 1;
	}

	if (errFlag) {
		free(pixMap);
		return -1;
	}

	free(pixMap);

	FILE *posFile;
	posFile = fopen("Pos.txt","w");
	if (posFile == NULL) {
		printf("Error : cannot open Pos.txt\n");
		fclose(posFile);
		return -1;
	}

	fprintf(posFile,"Red: %d, %d, %d\nYellow: %d, %d, %d\nWhite: %d, %d, %d",redPosition.x,redPosition.y,redPosition.score,yellowPosition.x,yellowPosition.y,yellowPosition.score,whitePosition.x,whitePosition.y,whitePosition.score);

	fclose(posFile);
	

	time_t end = clock();
	double run_time = (float)(end-begin)*1000/CLOCKS_PER_SEC;
	printf("%lf ms\n",run_time);

	return 0;
}

//Other functions writing

struct color intToColor(int colorInteger) {
	struct color colorOutput;
	colorOutput.blue  = colorInteger % 256;
	colorOutput.green = (colorInteger/256) % 256;
	colorOutput.red   = (colorInteger/65536) % 256;
	return colorOutput;
}

int getScore(unsigned int *pixMap, int index, struct color minRange, struct color maxRange, int imageHeight, int imageWidth, int ballDiameter, char mode) {

	int score = 0;
	struct color pixColor={0,0,0};

	if (mode=='s') {

		//Verifying that all the area we will check for is within the limits of the image
		if (index%imageWidth > imageWidth-ballDiameter || index/imageWidth > imageHeight-ballDiameter) {
			printf("Error : trying to get score too close to image limits, trying to access pixels outside image\n");
			return -1;
		};

		//Scanning the area of interest for pixels within the range, and incrementing the score for each valid pixel
		int xPos, yPos;
		for (xPos = index % imageWidth ; xPos < index%imageWidth + ballDiameter ; xPos++) {
			for (yPos = index/imageWidth ; yPos < index/imageWidth + ballDiameter ; yPos++) {
				pixColor = intToColor(pixMap[xPos + yPos*imageWidth]);
				if (pixColor.red >= minRange.red && pixColor.red <= maxRange.red && pixColor.green >= minRange.green && pixColor.green <= maxRange.green && pixColor.blue >= minRange.blue && pixColor.blue <= maxRange.blue) score++;
			}
		}
		return score;

	} else if(mode=='c') {

		//Verifying that all the area we will check for is within the limits of the image
		if (index%imageWidth >= imageWidth-1 || index/imageWidth >= imageHeight-1 || index%imageWidth <= 0 || index/imageWidth <= 0) {
			printf("Error : trying to get score too close to image limits, trying to access pixels outside image\n");
			return -1;
		};

		pixColor = intToColor(pixMap[index]);
		if (pixColor.red >= minRange.red && pixColor.red <= maxRange.red && pixColor.green >= minRange.green && pixColor.green <= maxRange.green && pixColor.blue >= minRange.blue && pixColor.blue <= maxRange.blue) score++;
		pixColor = intToColor(pixMap[index+1]);
		if (pixColor.red >= minRange.red && pixColor.red <= maxRange.red && pixColor.green >= minRange.green && pixColor.green <= maxRange.green && pixColor.blue >= minRange.blue && pixColor.blue <= maxRange.blue) score++;
		pixColor = intToColor(pixMap[index-1]);
		if (pixColor.red >= minRange.red && pixColor.red <= maxRange.red && pixColor.green >= minRange.green && pixColor.green <= maxRange.green && pixColor.blue >= minRange.blue && pixColor.blue <= maxRange.blue) score++;
		pixColor = intToColor(pixMap[index+imageWidth]);
		if (pixColor.red >= minRange.red && pixColor.red <= maxRange.red && pixColor.green >= minRange.green && pixColor.green <= maxRange.green && pixColor.blue >= minRange.blue && pixColor.blue <= maxRange.blue) score++;
		pixColor = intToColor(pixMap[index-imageWidth]);
		if (pixColor.red >= minRange.red && pixColor.red <= maxRange.red && pixColor.green >= minRange.green && pixColor.green <= maxRange.green && pixColor.blue >= minRange.blue && pixColor.blue <= maxRange.blue) score++;

		return score;
	} else {
		printf("Error : invalid functionning mode for getScore");
		return -1;
	}
}

struct vectPlus convergence(unsigned int *pixMap, int index, struct color minRange, struct color maxRange, int imageHeight, int imageWidth, int ballDiameter) {

	int scoreUpdate;
	struct vectPlus outVect = {0,0,0};

	outVect.score = getScore(pixMap,index,minRange,maxRange,imageHeight,imageWidth,ballDiameter,'s');

	scoreUpdate = getScore(pixMap,index+1,minRange,maxRange,imageHeight,imageWidth,ballDiameter,'s');
	if (scoreUpdate > outVect.score) {
		return convergence(pixMap,index+1,minRange,maxRange,imageHeight,imageWidth,ballDiameter);
	}

	scoreUpdate = getScore(pixMap,index-1,minRange,maxRange,imageHeight,imageWidth,ballDiameter,'s');
	if (scoreUpdate > outVect.score) {
		return convergence(pixMap,index-1,minRange,maxRange,imageHeight,imageWidth,ballDiameter);
	}

	scoreUpdate = getScore(pixMap,index+imageWidth,minRange,maxRange,imageHeight,imageWidth,ballDiameter,'s');
	if (scoreUpdate > outVect.score) {
		return convergence(pixMap,index+imageWidth,minRange,maxRange,imageHeight,imageWidth,ballDiameter);
	}

	scoreUpdate = getScore(pixMap,index-imageWidth,minRange,maxRange,imageHeight,imageWidth,ballDiameter,'s');
	if (scoreUpdate > outVect.score) {
		return convergence(pixMap,index-imageWidth,minRange,maxRange,imageHeight,imageWidth,ballDiameter);
	}

	outVect.x = index%imageWidth;
	outVect.y = index/imageWidth;

	return outVect;

}