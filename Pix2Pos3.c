#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
	/*Results*/
		struct coordinate Red = {-1, -1, -1};
		struct coordinate Yellow = {-1, -1, -1};
		struct coordinate White = {-1, -1, -1};
	/*Program arguments*/
		short signed int BallDiameter = 11;
		struct coordinate TableMin = {15, 15, 0};
		struct coordinate TableMax = {85, 65, 0};
		struct colour RBallMin = {160, 0, 0};
		struct colour RBallMax = {255, 160, 160};
		struct colour YBallMin = {140, 140, 0};
		struct colour YBallMax = {255, 255, 175};
		struct colour WBallMin = {100, 100, 100};
		struct colour WBallMax = {255, 255, 255};
		//struct colour BGMin = {39, 91, 202};
		//struct colour BGMax = {62, 116, 255};
		struct colour BGMin = {0, 120, 200};
		struct colour BGMax = {10, 130, 255};
	/*Tiles*/
		struct coordinate TileAmount;
		struct coordinate *Tiles;
	/*Files and size*/
		int PixelWidth = 0, PixelHeight = 0;
		FILE *PixmapBin;
		FILE *PosTxt;
	/*Pixelmap*/
		unsigned int *PixelInt;
		//
	/*Errorflag*/
		short int ErrorFlag = 0;
		//


/* Function definition */
	/*Exits the program*/
void Exit(int status){
	printf("test\n");
	free(Tiles);
	printf("test\n");
	free(PixelInt);
	printf("test\n");
	fclose(PosTxt);
	printf("test\n");
	exit(status);
}
	/*Converting a decimal colour to a RGB colour*/
struct colour Int2Colour(int ColourInt) {
	struct colour ColourRGB;
	/*Detecting if int not in correct range*/
		if(ColourInt < 0 || ColourInt > 16777215){
			ColourRGB.R = -1;
			ColourRGB.G = -1;
			ColourRGB.B = -1;
			return ColourRGB;
		}
	/*Calculating colour*/
		ColourRGB.B = ColourInt % 256;
		ColourInt /= 256;
		ColourRGB.G = ColourInt % 256;
		ColourInt /= 256;
		ColourRGB.R = ColourInt;
	return ColourRGB;
}
	/*Calculating the amount of pixels that correspond to the given range*/
int GetScore(struct coordinate Coordinates, int DeltaX, int DeltaY, struct colour RangeMin, struct colour RangeMax){
	int Score = 0;
	for(int x = Coordinates.X; x < Coordinates.X + DeltaX; x++){
		for(int y = Coordinates.Y; y < Coordinates.Y + DeltaY; y++){
			struct colour PixelColour = Int2Colour(PixelInt[x + y*PixelWidth]);
			if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) printf("Error : colour error at pixel %d ignoring pixel\n", x + y*PixelWidth);
			if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
		}
	}
	return Score;
}
	/*Tiles the map with tiles of size Balldiameter*/
void MapTile(){
	for(int TileY = 0; TileY < TileAmount.Y; TileY++){
		for(int TileX = 0; TileX < TileAmount.X; TileX++){
			int index = TileY*TileAmount.X + TileX;
			int x = TableMin.X + TileX*BallDiameter;
			if(x+BallDiameter > TableMax.X) x -= x + BallDiameter - TableMax.X;
			int y = TableMin.Y + TileY*BallDiameter;
			if(y+BallDiameter > TableMax.Y) y -= y + BallDiameter - TableMax.Y;
			Tiles[index].X = x;
			Tiles[index].Y = y;
		}
	}
}
	/*Makes the tiles converge to the maximum score*/
void Converge(struct colour RangeMin, struct colour RangeMax){
	/*Calculating first score in a smaller 2*2 square*/
		for(int index = 0; index < TileAmount.X * TileAmount.Y; index++) Tiles[index].Score = GetScore(Tiles[index], 2, 2, RangeMin, RangeMax);
		//
	/*Iterating through the tiles and move those who have a score to a place with bigger score*/
		int flag = 1;
		while(flag){
			flag = 0;
			for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
				if(Tiles[index].Score){
					/*Evaluating the possible movements*/
						Tiles[index].Y++;
						int up = GetScore(Tiles[index], BallDiameter, BallDiameter, RangeMin, RangeMax);
						Tiles[index].Y -= 2;
						int down = GetScore(Tiles[index], BallDiameter, BallDiameter, RangeMin, RangeMax);
						Tiles[index].Y++;
						Tiles[index].X++;
						int right = GetScore(Tiles[index], BallDiameter, BallDiameter, RangeMin, RangeMax);
						Tiles[index].X -= 2;
						int left = GetScore(Tiles[index], BallDiameter, BallDiameter, RangeMin, RangeMax);
						Tiles[index].X++;
					/*Move to the best spot*/
						if(up > Tiles[index].Score){
							Tiles[index].Y++;
							Tiles[index].Score = up;
							flag = 1;
						}
						if(down > Tiles[index].Score){
							Tiles[index].Y--;
							Tiles[index].Score = down;
							flag = 1;
						}
						if(right > Tiles[index].Score){
							Tiles[index].X++;
							Tiles[index].Score = right;
							flag = 1;
						}
						if(left > Tiles[index].Score){
							Tiles[index].X--;
							Tiles[index].Score = left;
							flag = 1;
						}
				}
			}
		}
}
	/*Find the balls*/
void CheckForBalls(){
	/*Find the red balls*/
		MapTile();
		Converge(RBallMin, RBallMax);
	/*Check if there is only one and save it*/
		for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
			if(Tiles[index].Score){
				if(Red.X < 0){
					Red.X = Tiles[index].X;
					Red.Y = Tiles[index].Y;
					Red.Score = Tiles[index].Score;
				}
				else{
					if(Red.X != Tiles[index].X || Red.Y != Tiles[index].Y){
						printf("Error : multiple red balls, continuing with x=%d and y=%d\n", Red.X, Red.Y);
						Exit(-1);
					}
				}
			}
		}
	/*Find the yellow balls*/
		MapTile();
		Converge(YBallMin, YBallMax);
	/*Check if there is only one and save it*/
		for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
			if(Tiles[index].Score){
				if(Yellow.X < 0){
					Yellow.X = Tiles[index].X;
					Yellow.Y = Tiles[index].Y;
					Yellow.Score = Tiles[index].Score;
				}
				else{
					if(Yellow.X != Tiles[index].X || Yellow.Y != Tiles[index].Y){
						printf("Error : multiple yellow balls, cannot continue\n");
						Exit(-1);
					}
				}
			}
		}
	/*Find the white balls*/
		MapTile();
		Converge(WBallMin, WBallMax);
	/*Check if there is only one and save it*/
		for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
			if(Tiles[index].Score){
				if(White.X < 0){
					White.X = Tiles[index].X;
					White.Y = Tiles[index].Y;
					White.Score = Tiles[index].Score;
				}
				else{
					if(White.X != Tiles[index].X || White.Y != Tiles[index].Y){
						printf("Error : multiple white balls, cannot continue\n");
						Exit(-1);
					}
				}
			}
		}
		//printf("Red:%d,%d,%d\nYellow:%d,%d,%d\nWhite:%d,%d,%d\n", Red.X, Red.Y, Red.Score, Yellow.X, Yellow.Y, Yellow.Score, White.X, White.Y, White.Score);
	/*Check if the balls are overlapping*/
		if((Red.X + 11 > Yellow.X && Red.X - 11 < Yellow.X) && (Red.Y + 11 > Yellow.Y && Red.Y - 11 < Yellow.Y)){
			printf("Error : red ball and yellow ball overlapping, cannot continue\n");
			ErrorFlag = 1;
		}
		if((Red.X + 11 > White.X && Red.X - 11 < White.X) && (Red.Y + 11 > White.Y && Red.Y - 11 < White.Y)){
			printf("Error : red ball and white ball overlapping, cannot continue\n");
			ErrorFlag = 1;
		}
		if((White.X + 11 > Yellow.X && White.X - 11 < Yellow.X) && (White.Y + 11 > Yellow.Y && White.Y - 11 < Yellow.Y)){
			printf("Error : white ball and yellow ball overlapping, cannot continue\n");
			ErrorFlag = 1;
		}
		if(ErrorFlag) Exit(-1);
}
	/*Find the table*/
void FindTable(){
	/*Find the top*/
		ErrorFlag = 1;
		for(int y = TableMin.Y; y < PixelHeight; y++){
			struct coordinate Coordinates = {PixelWidth/2, y, 0};
			if(GetScore(Coordinates, 1, 1, BGMin, BGMax)){
				TableMin.Y = y-1;
				ErrorFlag = 0;
				break;
			}
		}
	/*Find the left*/
		ErrorFlag = 1;
		for(int x = TableMin.X; x < PixelWidth; x++){
			struct coordinate Coordinates = {x, PixelHeight/2, 0};
			if(GetScore(Coordinates, 1, 1, BGMin, BGMax)){
				TableMin.X = x-1;
				ErrorFlag = 0;
				break;
			}
		}
	/*Find the bottom*/
		ErrorFlag = 1;
		for(int y = TableMax.Y; y > 0; y--){
			struct coordinate Coordinates = {PixelWidth/2, y, 0};
			if(GetScore(Coordinates, 1, 1, BGMin, BGMax)){
				TableMax.Y = y;
				ErrorFlag = 0;
				break;
			}
		}
	/*Find the right*/
		ErrorFlag = 1;
		for(int x = TableMax.X; x > 0; x--){
			struct coordinate Coordinates = {x, PixelHeight/2, 0};
			if(GetScore(Coordinates, 1, 1, BGMin, BGMax)){
				TableMax.X = x;
				ErrorFlag = 0;
				break;
			}
		}
	/*Check if everything went fine*/
		if(ErrorFlag){
			printf("Error : Table not found\n");
			Exit(-1);
		}
}


/*Main*/
int main(int argc, char **argv){

	time_t begin = clock();
	Tiles = malloc(1);
	//PixelInt = malloc(1);
/*read arguments*/
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
		printf("Error : invalid number of argument, cannot continue\n");
		//ErrorFlag = 1;
	}
/*check first errors*/
	if(TableMin.X > TableMax.X || TableMin.Y > TableMax.Y || TableMin.X < 0 || TableMin.Y < 0){
		printf("Error : invalid values passed as table size, cannot continue\n");
		ErrorFlag = 1;
	}
	if(RBallMin.R > RBallMax.R || RBallMin.G > RBallMax.G || RBallMin.B > RBallMax.B || RBallMin.R < 0 || RBallMin.G < 0 || RBallMin.B < 0){
		printf("Error : invalid values passed as red ball colour range, cannot continue\n");
		ErrorFlag = 1;
	}
	if(YBallMin.R > YBallMax.R || YBallMin.G > YBallMax.G || YBallMin.B > YBallMax.B || YBallMin.R < 0 || YBallMin.G < 0 || YBallMin.B < 0){
		printf("Error : invalid values passed as yellow ball colour range, cannot continue\n");
		ErrorFlag = 1;
	}
	if(WBallMin.R > WBallMax.R || WBallMin.G > WBallMax.G || WBallMin.B > WBallMax.B || WBallMin.R < 0 || WBallMin.G < 0 || WBallMin.B < 0){
		printf("Error : invalid values passed as white ball colour range, cannot continue\n");
		ErrorFlag = 1;
	}
	if(BGMin.R > BGMax.R || BGMin.G > BGMax.G || BGMin.B > BGMax.B || BGMin.R < 0 || BGMin.G < 0 || BGMin.B < 0){
		printf("Error : invalid values passed as background colour range, cannot continue\n");
		ErrorFlag = 1;
	}
	if(TableMax.X - TableMin.X < BallDiameter || TableMax.Y - TableMin.Y < BallDiameter){
		printf("Error : invalid values passed as table and ball size, ball is bigger than table, cannot continue\n");
		ErrorFlag = 1;
	}
	if(BallDiameter < 5 || BallDiameter > 20){
		printf("Error : invalid values passed as ball size, cannot continue\n");
		ErrorFlag = 1;
	}
	if(ErrorFlag) Exit(-1);
/*Open Pixmap.bin and verify width and heigth*/
	PixmapBin = fopen("Pixmap.bin", "r");
	if(PixmapBin == NULL){
		printf("Error : couldn't open Pixmap.bin");
		Exit(-1);
	}
	if(1 != fread(&PixelWidth, sizeof(unsigned int), 1, PixmapBin)){
		printf("Error : couldn't read image width, cannot continue\n");
		ErrorFlag = 1;
	}
	if(1 != fread(&PixelHeight, sizeof(unsigned int), 1, PixmapBin)){
		printf("Error : couldn't read image height, cannot continue\n");
		ErrorFlag = 1;
	}
	if(PixelWidth < 10 || PixelHeight < 10 || PixelWidth > 1000 || PixelHeight > 1000){
		printf("Error : invalid values passed as image size, cannot continue\n");
		ErrorFlag = 1;
	}
	if(ErrorFlag) Exit(-1);
/*Read the pixels and close the file*/
	PixelInt = malloc(sizeof(unsigned int)*(PixelWidth*PixelHeight+1));
	//realloc(PixelInt, sizeof(unsigned int)*(PixelWidth*PixelHeight+1));
	int temp = fread(PixelInt, sizeof(unsigned int), PixelWidth*PixelHeight+1, PixmapBin);
	if(temp < PixelWidth*PixelHeight){
		printf("Error : not enough pixel, cannot continue\n");
		Exit(-1);
	}
	else if(temp > PixelWidth*PixelHeight){
		printf("Error : too many pixels, cannot continue\n");
		Exit(-1);
	}
	fclose(PixmapBin);
/*Find the table*/
	FindTable();
	printf("Table found\n");
/*create the tiles*/
	TileAmount.X = (TableMax.X-TableMin.X) / BallDiameter + ((TableMax.X-TableMin.X) % BallDiameter == 0 ? 0: 1);
	TileAmount.Y = (TableMax.Y-TableMin.Y) / BallDiameter + ((TableMax.Y-TableMin.Y) % BallDiameter == 0 ? 0: 1);
	realloc(Tiles, sizeof(struct coordinate)*TileAmount.X * TileAmount.Y);
/*find the balls*/
	CheckForBalls();	//
	printf("Balls found\n");
/*check if the balls are all here*/
	if(Red.X < 0){
		printf("Error : red ball missing\n");
		ErrorFlag = 1;
	}
	if(Yellow.X < 0){
		printf("Error : yellow ball missing\n");
		ErrorFlag = 1;
	}
	if(White.X < 0){
		printf("Error : white ball missing\n");
		ErrorFlag = 1;
	}
	if(ErrorFlag) Exit(-1);
/*open and write in Pos.txt*/
	PosTxt = fopen("Pos.txt", "w");
	if(PosTxt == NULL){
		printf("Error : couldn't open Pos.txt\n");
		Exit(-1);
	}
	fprintf(PosTxt, "Red:%d,%d,%d\nYellow:%d,%d,%d\nWhite:%d,%d,%d\n", Red.X, Red.Y, Red.Score, Yellow.X, Yellow.Y, Yellow.Score, White.X, White.Y, White.Score);
/**/
	printf("Program executed\n");

	time_t end = clock();
	float time_of_execution = ((double)end-(double)begin)*1000/CLOCKS_PER_SEC;
	printf("%lf ms",time_of_execution);
	Exit(0);
}