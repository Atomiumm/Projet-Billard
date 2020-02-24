#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PM.h"		//provisoire


/* Structure definition */
struct colour {
	signed short int R;
	signed short int G;
	signed short int B;
};

struct coordinate {
	signed short int X;
	signed short int Y;
	signed short int Score;
};


/* Global Variables */
	struct coordinate Red = {0, 0, 0};
	struct coordinate Yellow = {0, 0, 0};
	struct coordinate White = {0, 0, 0};
	
	short signed int BallDiameter = 11;
	struct coordinate TableMin = {15, 15, 0};
	struct coordinate TableMax = {85, 65, 0};
	struct colour RBallMin = {160, 0, 0};
	struct colour RBallMax = {255, 160, 160};
	struct colour YBallMin = {140, 140, 0};
	struct colour YBallMax = {255, 255, 175};
	struct colour WBallMin = {100, 100, 100};
	struct colour WBallMax = {255, 255, 255};
	struct colour BGMin = {39, 91, 202};
	struct colour BGMax = {62, 116, 255};


/* Function definition */
struct colour Int2Colour(int ColourInt) {
	struct colour ColourRGB;
	if(ColourInt < 0 || ColourInt >= 16777216){
		ColourRGB.R = -1;
		ColourRGB.G = -1;		//Ca me fait chier et on doit changer
		ColourRGB.B = -1;
		return ColourRGB;
	}
	ColourRGB.B = ColourInt % 256;
	ColourInt /= 256;
	ColourRGB.G = ColourInt % 256;
	ColourInt /= 256;
	ColourRGB.R = ColourInt;
	return ColourRGB;
}

int GetScore(struct coordinate Coordinates, int DeltaX, int DeltaY, struct colour RangeMin, struct colour RangeMax){
	int Score = 0;
	for(int x = Coordinates.X; x < Coordinates.X + DeltaX; x++){
		for(int y = Coordinates.Y; y < Coordinates.Y + DeltaY; y++){
			struct colour PixelColour = Int2Colour(myPM[x + y*myW]);
			if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) printf("Error : colour error at pixel %d ignoring pixel\n", x + y*myW);
			if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
		}
	}
	return Score;
}

void Modify_Results(struct coordinate Coordinates){
	int scoreR = GetScore(Coordinates, BallDiameter, BallDiameter, RBallMin, RBallMax);
	if(scoreR > Red.Score){
		Red.X = Coordinates.X;
		Red.Y = Coordinates.Y;
		Red.Score = scoreR;
	}
	int scoreY = GetScore(Coordinates, BallDiameter, BallDiameter, YBallMin, YBallMax);
	if(scoreY > Yellow.Score){
		Yellow.X = Coordinates.X;
		Yellow.Y = Coordinates.Y;
		Yellow.Score = scoreY;
	}
	int scoreW = GetScore(Coordinates, BallDiameter, BallDiameter, WBallMin, WBallMax);
	if(scoreW > White.Score){
		White.X = Coordinates.X;
		White.Y = Coordinates.Y;
		White.Score = scoreW;
	}
}


/* Main */
int main(int argc, char **argv) {

	if(argc == 30){
		TableMin.X   = atoi(argv[1]);
		TableMax.X   = atoi(argv[2]);
		TableMin.Y   = atoi(argv[3]);
		TableMax.Y   = atoi(argv[4]);
		RBallMin.R   = atoi(argv[5]);
		RBallMax.R   = atoi(argv[6]);
		RBallMin.G   = atoi(argv[7]);
		RBallMax.G   = atoi(argv[8]);
		RBallMin.B   = atoi(argv[9]);
		RBallMax.B   = atoi(argv[10]);
		YBallMin.R   = atoi(argv[11]);
		YBallMax.R   = atoi(argv[12]);
		YBallMin.G   = atoi(argv[13]);
		YBallMax.G   = atoi(argv[14]);
		YBallMin.B   = atoi(argv[15]);
		YBallMax.B   = atoi(argv[16]);
		WBallMin.R   = atoi(argv[17]);
		WBallMax.R   = atoi(argv[18]);
		WBallMin.G   = atoi(argv[19]);
		WBallMax.G   = atoi(argv[20]);
		WBallMin.B   = atoi(argv[21]);
		WBallMax.B   = atoi(argv[22]);
		BGMin.R      = atoi(argv[23]);
		BGMax.R      = atoi(argv[24]);
		BGMin.G      = atoi(argv[25]);
		BGMax.G      = atoi(argv[26]);
		BGMin.B      = atoi(argv[27]);
		BGMax.B      = atoi(argv[28]);
		BallDiameter = atoi(argv[29]);
	}
	else {
		printf("Error : invalid number of argument, continuing with default values\n");
	}

	int flag = 0;
	if(TableMin.X > TableMax.X || TableMin.Y > TableMax.Y || TableMin.X < 0 || TableMin.Y < 0){
		printf("Error : invalid values passed as table size, cannot continue\n");
		flag = 1;
	}
	if(RBallMin.R > RBallMax.R || RBallMin.G > RBallMax.G || RBallMin.B > RBallMax.B || RBallMin.R < 0 || RBallMin.G < 0 || RBallMin.B < 0){
		printf("Error : invalid values passed as red ball colour range, cannot continue\n");
		flag = 1;
	}
	if(YBallMin.R > YBallMax.R || YBallMin.G > YBallMax.G || YBallMin.B > YBallMax.B || YBallMin.R < 0 || YBallMin.G < 0 || YBallMin.B < 0){
		printf("Error : invalid values passed as yellow ball colour range, cannot continue\n");
		flag = 1;
	}
	if(WBallMin.R > WBallMax.R || WBallMin.G > WBallMax.G || WBallMin.B > WBallMax.B || WBallMin.R < 0 || WBallMin.G < 0 || WBallMin.B < 0){
		printf("Error : invalid values passed as white ball colour range, cannot continue\n");
		flag = 1;
	}
	if(BGMin.R > BGMax.R || BGMin.G > BGMax.G || BGMin.B > BGMax.B || BGMin.R < 0 || BGMin.G < 0 || BGMin.B < 0){
		printf("Error : invalid values passed as background colour range, cannot continue\n");
		flag = 1;
	}
	if(BallDiameter < 0){
		printf("Error : invalid values passed as ball size, cannot continue\n");
	}
	if(TableMax.X - TableMin.X < BallDiameter || TableMax.Y - TableMin.Y < BallDiameter){
		printf("Error : invalid values passed as table and ball size, ball is bigger than table, cannot continue\n");
		flag = 1;
	}	
	if(flag) return 0;



	for(int x = TableMin.X; x <= TableMax.X - BallDiameter; x++){
		for(int y = TableMin.Y; y <= TableMax.Y - BallDiameter; y++){
			struct coordinate PixelCoords = {x, y, 0};
			Modify_Results(PixelCoords);
		}
	}



	printf("Red: %d, %d, %d\n", Red.X, Red.Y, Red.Score);
	printf("Yellow: %d, %d, %d\n", Yellow.X, Yellow.Y, Yellow.Score);
	printf("White: %d, %d, %d\n", White.X, White.Y, White.Score);


	return 0;
}













