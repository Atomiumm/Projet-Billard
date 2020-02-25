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
	struct coordinate Red = {-1, -1, -1};
	struct coordinate Yellow = {-1, -1, -1};
	struct coordinate White = {-1, -1, -1};
	
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
	//struct colour LimMin = {0, 0, 121};
	//struct colour LimMax = {14, 42, 126};
	struct colour LimMin = {0, 0, 121};
	struct colour LimMax = {0, 54, 180};

	struct coordinate TileAmount;
	struct coordinate *Tiles;

	int PixelWidth = 0, PixelHeight = 0;
	FILE *PixmapBin;
	FILE *PosTxt;

	unsigned int *PixelInt;

/* Function definition */
struct colour Int2Colour(int ColourInt) {
	struct colour ColourRGB;
	if(ColourInt < 0 || ColourInt > 16777215){
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

void MapTile(){
	for(int TileY = 0; TileY - TileAmount.Y; TileY++){
		for(int TileX = 0; TileX - TileAmount.X; TileX++){
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
  
void Converge(struct colour RangeMin, struct colour RangeMax){
	for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
		Tiles[index].Score = GetScore(Tiles[index], BallDiameter, BallDiameter, RangeMin, RangeMax);
	}
	int flag = 1;
	while(flag){
		flag = 0;
		for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
			if(Tiles[index].Score){
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

void CheckForBalls(){
	MapTile();
	Converge(RBallMin, RBallMax);
	for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
		if(Tiles[index].Score){
			if(Red.X < 0){
				Red.X = Tiles[index].X;
				Red.Y = Tiles[index].Y;
			}
			else{
				if(Red.X != Tiles[index].X || Red.Y != Tiles[index].Y) printf("Error : multiple red balls, continuing with x=%d and y=%d\n", Red.X, Red.Y);
			}
		}
	}
	MapTile();
	Converge(YBallMin, YBallMax);
	for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
		if(Tiles[index].Score){
			if(Yellow.X < 0){
				Yellow.X = Tiles[index].X;
				Yellow.Y = Tiles[index].Y;
			}
			else{
				if(Yellow.X != Tiles[index].X || Yellow.Y != Tiles[index].Y) printf("Error : multiple yellow balls, continuing with x=%d and y=%d\n", Yellow.X, Yellow.Y);
			}
		}
	}
	MapTile();
	Converge(WBallMin, WBallMax);
	for(int index = 0; index < TileAmount.X * TileAmount.Y; index++){
		if(Tiles[index].Score){
			if(White.X < 0){
				White.X = Tiles[index].X;
				White.Y = Tiles[index].Y;
			}
			else{
				if(White.X != Tiles[index].X || White.Y != Tiles[index].Y) printf("Error : multiple white balls, continuing with x=%d and y=%d\n", White.X, White.Y);
			}
		}
	}
}

void FindTable(){
	for(int y = 0; y < myH; y++){
		struct coordinate Coordinates = {myW/2, y, 0};
		if(GetScore(Coordinates, 1, 1, LimMin, LimMax)){
			TableMin.Y = y;
			break;
		}
	}
	for(int x = 0; x < myW; x++){
		struct coordinate Coordinates = {x, myH/2, 0};
		if(GetScore(Coordinates, 1, 1, LimMin, LimMax)){
			TableMin.X = x;
			break;
		}
	}
	for(int y = myH; y > 0; y--){
		struct coordinate Coordinates = {myW/2, y, 0};
		if(GetScore(Coordinates, 1, 1, LimMin, LimMax)){
			TableMax.Y = y+1;
			break;
		}
	}
	for(int x = myW; x > 0; x--){
		struct coordinate Coordinates = {x, myH/2, 0};
		if(GetScore(Coordinates, 1, 1, LimMin, LimMax)){
			TableMax.X = x+1;
			break;
		}
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
		flag = 1;
	}
	if(TableMax.X - TableMin.X < BallDiameter || TableMax.Y - TableMin.Y < BallDiameter){
		printf("Error : invalid values passed as table and ball size, ball is bigger than table, cannot continue\n");
		flag = 1;
	}
	if(myW < 10 || myH < 10 || myW > 1000 || myH > 1000){
		printf("Error : invalid values passed as image size, cannot continue\n");
		flag = 1;
	}
	if(BallDiameter < 5 || BallDiameter > 20){
		printf("Error : invalid values passed as image size, cannot continue\n");
		flag = 1;
	}
	if(sizeof(myPM)/sizeof(int) < myH*myW){
		printf("Error : invalid values passed as image size, cannot continue\n");
		flag = 1;
	}

	PixmapBin = fopen("Pixmap.bin", "r");
	if(PixmapBin == NULL){
		printf("Error : couldn't open Pixmap.bin");
		return 0;
	}
	int len = 1;
	for(char c = getc(PixmapBin); c != EOF; c = getc(PixmapBin)) 
        if (c == '\n')
            len = len + 1; 
    PixelInt = (int *) malloc(len-2);
	if(1 != fread(&PixelWidth, sizeof(unsigned int), 1, PixmapBin)){
		printf("Error : couldn't read image width, cannot continue\n");
		flag = 1;
	}
	if(1 != fread(&PixelHeight, sizeof(unsigned int), 1, PixmapBin)){
		printf("Error : couldn't read image height, cannot continue\n");
		flag = 1;
	}

    printf("%d, %d\n", PixelWidth, PixelHeight);

	fclose(PixmapBin);


	if(flag) return 0;






	FindTable();
	printf("TableMin: %d, %d, %d\n", TableMin.X, TableMin.Y, TableMin.Score);
	printf("TableMax: %d, %d, %d\n", TableMax.X, TableMax.Y, TableMax.Score);

	TileAmount.X = (TableMax.X-TableMin.X) / BallDiameter + ((TableMax.X-TableMin.X) % BallDiameter == 0 ? 0: 1);
	TileAmount.Y = (TableMax.Y-TableMin.Y) / BallDiameter + ((TableMax.Y-TableMin.Y) % BallDiameter == 0 ? 0: 1);
	Tiles = (struct coordinate *) malloc(TileAmount.X * TileAmount.Y);

	CheckForBalls();




	if(Red.X < 0) printf("Error : red ball missing\n");
	else printf("Red: %d, %d, %d\n", Red.X, Red.Y, Red.Score);
	if(Yellow.X < 0) printf("Error : yellow ball missing\n");
	else printf("Yellow: %d, %d, %d\n", Yellow.X, Yellow.Y, Yellow.Score);
	if(White.X < 0) printf("Error : white ball missing\n");
	else printf("White: %d, %d, %d\n", White.X, White.Y, White.Score);

	free(Tiles);

	return 0;
}













