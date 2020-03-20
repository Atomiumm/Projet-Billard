#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_BALL_DIAMETER 5
#define MAX_BALL_DIAMETER 20
#define MIN_IMAGE_WIDTH 10
#define MIN_IMAGE_HEIGHT 10
#define MAX_IMAGE_WIDTH 1000
#define MAX_IMAGE_HEIGHT 1000


/*Structure declaration*/
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



/*Function declaration*/
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
	/*Calculating the amount of pixels that correspond to the given range.	The last parameter (mode) can be two -> very fast calculation or one -> fast calculation or 0 -> precise calculation*/
int GetScore(unsigned int *PixelInt, int PixelWidth, struct coordinate Coordinates, int DeltaX, int DeltaY, struct colour RangeMin, struct colour RangeMax, short int Mode){
	int Score = 0;
	if(Mode == 2){
		struct colour PixelColour = Int2Colour(PixelInt[(Coordinates.X + DeltaX/4) + (Coordinates.Y + DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + DeltaX/4)+(Coordinates.Y + DeltaY/4)*PixelWidth);
		if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
		PixelColour = Int2Colour(PixelInt[(Coordinates.X + 3*DeltaX/4) + (Coordinates.Y + DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + 3*DeltaX/4)+(Coordinates.Y + DeltaY/4)*PixelWidth);
		if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
		PixelColour = Int2Colour(PixelInt[(Coordinates.X + DeltaX/4) + (Coordinates.Y + 3*DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + DeltaX/4)+(Coordinates.Y + 3*DeltaY/4)*PixelWidth);
		if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
		PixelColour = Int2Colour(PixelInt[(Coordinates.X + 3*DeltaX/4) + (Coordinates.Y + 3*DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + 3*DeltaX/4)+(Coordinates.Y + 3*DeltaY/4)*PixelWidth);
		if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
	}
	else if(Mode == 1){
		int x = Coordinates.X + DeltaX/2;
		for(int y = Coordinates.Y; y < Coordinates.Y + DeltaY; y++){
			struct colour PixelColour = Int2Colour(PixelInt[x + y*PixelWidth]);
			if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", x + y*PixelWidth);
			if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
		}
		int y = Coordinates.Y + DeltaY/2;
		for(int x = Coordinates.X; x < Coordinates.X + DeltaX; x++){
			struct colour PixelColour = Int2Colour(PixelInt[x + y*PixelWidth]);
			if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", x + y*PixelWidth);
			if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
		}
	}
	else{
		for(int x = Coordinates.X; x < Coordinates.X + DeltaX; x++){
			for(int y = Coordinates.Y; y < Coordinates.Y + DeltaY; y++){
				struct colour PixelColour = Int2Colour(PixelInt[x + y*PixelWidth]);
				if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", x + y*PixelWidth);
				if(PixelColour.R >= RangeMin.R && PixelColour.R <= RangeMax.R && PixelColour.G >= RangeMin.G && PixelColour.G <= RangeMax.G && PixelColour.B >= RangeMin.B && PixelColour.B <= RangeMax.B) Score++;
			}
		}
	}
	return Score;
}
	/*Converging to a better position*/
void Converge(unsigned int *PixelInt, int PixelWidth, struct coordinate *PCoordinate, int BallDiameter, struct colour RangeMin, struct colour RangeMax){
	PCoordinate->X++;
	int tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->X-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->X++;
	PCoordinate->Y++;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->Y-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->Y++;
	PCoordinate->X++;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->X-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->X++;
	PCoordinate->Y++;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->Y-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, RangeMin, RangeMax, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, RangeMin, RangeMax);
		return;
	}
	PCoordinate->Y++;
}
	/*Finding the balls*/
void FindBall(unsigned int *PixelInt, int PixelWidth, struct coordinate *PBall, struct coordinate TableMax, struct coordinate TableMin, short signed int BallDiameter, struct colour RangeMin, struct colour RangeMax){
	struct coordinate TileAmount;
	TileAmount.X = (TableMax.X-TableMin.X) / BallDiameter + ((TableMax.X-TableMin.X) % BallDiameter == 0 ? 0: 1);
	TileAmount.Y = (TableMax.Y-TableMin.Y) / BallDiameter + ((TableMax.Y-TableMin.Y) % BallDiameter == 0 ? 0: 1);
	for(int TileY = 0; TileY < TileAmount.Y; TileY++){
		int y = TableMin.Y + TileY*BallDiameter;
		if(y+BallDiameter > TableMax.Y) y -= y + BallDiameter - TableMax.Y;
		for(int TileX = 0; TileX < TileAmount.X; TileX++){
			int x = TableMin.X + TileX*BallDiameter;
			if(x+BallDiameter > TableMax.X) x -= x + BallDiameter - TableMax.X;
			struct coordinate Tile = {x, y, 0};
			if(GetScore(PixelInt, PixelWidth, Tile, BallDiameter, BallDiameter, RangeMin, RangeMax, 2)){
				if(GetScore(PixelInt, PixelWidth, Tile, BallDiameter, BallDiameter, RangeMin, RangeMax, 0) > BallDiameter*BallDiameter/5){
					struct coordinate *PTile = &Tile;
					PTile->Score = GetScore(PixelInt, PixelWidth, Tile, BallDiameter, BallDiameter, RangeMin, RangeMax, 1);
					Converge(PixelInt, PixelWidth, PTile, BallDiameter, RangeMin, RangeMax);
					if(PBall->X < 0  && Tile.Score > 7*BallDiameter*BallDiameter/10){
						PBall->X = Tile.X;
						PBall->Y = Tile.Y;
						PBall->Score = Tile.Score;
					}
					else if((abs(PBall->X - Tile.X) > BallDiameter/2 || abs(PBall->Y - Tile.Y) > BallDiameter/2) && Tile.Score > 7*BallDiameter*BallDiameter/10){
						fprintf(stderr, "Error : multiple balls in range: %d,%d,%d : %d,%d,%d, cannot continue\n", RangeMin.R, RangeMin.G, RangeMin.B, RangeMax.R,RangeMax.G,RangeMax.B);
						//fprintf(stderr, "Ball1: %d,%d:%d; Ball2: %d,%d:%d", PBall->X, PBall->Y, PBall->Score, Tile.X, Tile.Y, Tile.Score);		
						free(PixelInt);
						exit(EXIT_FAILURE);
					}
				}
			}
		}
	}
}



/*Main*/
int main(int argc, char **argv){
	
	/*Variable Declaration*/
		struct coordinate TableMin;
		struct coordinate TableMax;
		struct colour RBallMin;
		struct colour RBallMax;
		struct colour YBallMin;
		struct colour YBallMax;
		struct colour WBallMin;
		struct colour WBallMax;
		struct colour BGMin;
		struct colour BGMax;
		short signed int BallDiameter;
	/*Reads arguments*/
		if(argc == 30){
			TableMin.Y   = atoi(argv[1]);
			TableMax.Y   = atoi(argv[2]);
			TableMin.X   = atoi(argv[3]);
			TableMax.X   = atoi(argv[4]);
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
			fprintf(stderr, "Error : invalid number of argument, cannot continue\n");
			exit(EXIT_FAILURE);
		}
	/*Check first errors*/
		short unsigned int ErrorFlag = 0;
		if(TableMin.X > TableMax.X || TableMin.Y > TableMax.Y || TableMin.X < 0 || TableMin.Y < 0){
			fprintf(stderr, "Error : invalid values passed as table size, cannot continue\n");
			ErrorFlag = 1;
		}
		if(RBallMin.R > RBallMax.R || RBallMin.G > RBallMax.G || RBallMin.B > RBallMax.B || RBallMin.R < 0 || RBallMin.G < 0 || RBallMin.B < 0){
			fprintf(stderr, "Error : invalid values passed as red ball colour range, cannot continue\n");
			ErrorFlag = 1;
		}
		if(YBallMin.R > YBallMax.R || YBallMin.G > YBallMax.G || YBallMin.B > YBallMax.B || YBallMin.R < 0 || YBallMin.G < 0 || YBallMin.B < 0){
			fprintf(stderr, "Error : invalid values passed as yellow ball colour range, cannot continue\n");
			ErrorFlag = 1;
		}
		if(WBallMin.R > WBallMax.R || WBallMin.G > WBallMax.G || WBallMin.B > WBallMax.B || WBallMin.R < 0 || WBallMin.G < 0 || WBallMin.B < 0){
			fprintf(stderr, "Error : invalid values passed as white ball colour range, cannot continue\n");
			ErrorFlag = 1;
		}
		if(BGMin.R > BGMax.R || BGMin.G > BGMax.G || BGMin.B > BGMax.B || BGMin.R < 0 || BGMin.G < 0 || BGMin.B < 0){
			fprintf(stderr, "Error : invalid values passed as background colour range, cannot continue\n");
			ErrorFlag = 1;
		}
		if(TableMax.X - TableMin.X < BallDiameter || TableMax.Y - TableMin.Y < BallDiameter){
			fprintf(stderr, "Error : invalid values passed as table and ball size, ball is bigger than table, cannot continue\n");
			ErrorFlag = 1;
		}
		if(BallDiameter < MIN_BALL_DIAMETER || BallDiameter > MAX_BALL_DIAMETER){
			fprintf(stderr, "Error : invalid values passed as ball size, cannot continue\n");
			ErrorFlag = 1;
		}
		if(ErrorFlag) exit(EXIT_FAILURE);
	/*Open Pixmap.bin and verify width and heigth*/
		int PixelWidth = 0;
		int PixelHeight = 0;
		FILE *PixmapBin;
		PixmapBin = fopen("Pixmap.bin", "rb");
		if(PixmapBin == NULL){
			perror("Error : couldn't open Pixmap.bin");
			exit(EXIT_FAILURE);
		}
		if(!fread(&PixelWidth, sizeof(unsigned int), 1, PixmapBin)){
			fprintf(stderr, "Error : couldn't read image width, cannot continue\n");
			if(feof(PixmapBin)) fprintf(stderr, "Cause: end of file reached\n");
			int err = ferror(PixmapBin);
			if(err){
				fprintf(stderr, "Cause: error %d\n", err);
				clearerr(PixmapBin);
			}
			exit(EXIT_FAILURE);
		}
		if(!fread(&PixelHeight, sizeof(unsigned int), 1, PixmapBin)){
			fprintf(stderr, "Error : couldn't read image height, cannot continue\n");
			if(feof(PixmapBin)) fprintf(stderr, "Cause: end of file reached\n");
			int err = ferror(PixmapBin);
			if(err){
				fprintf(stderr, "Cause: error %d\n", err);
				clearerr(PixmapBin);
			}
			exit(EXIT_FAILURE);
		}
		if(PixelWidth < MIN_IMAGE_WIDTH || PixelHeight < MIN_IMAGE_HEIGHT || PixelWidth > MAX_IMAGE_WIDTH || PixelHeight > MAX_IMAGE_HEIGHT){
			fprintf(stderr, "Error : invalid values passed as image size, cannot continue\n");
			exit(EXIT_FAILURE);
		}
	/*Read the pixels and close the file*/
		unsigned int *PixelInt;
		PixelInt = malloc(sizeof(unsigned int)*(PixelWidth*PixelHeight+1));
		int temp;
		temp = fread(PixelInt, sizeof(unsigned int), PixelWidth*PixelHeight+1, PixmapBin);
		if(temp != PixelWidth*PixelHeight){
			fprintf(stderr, "Read %d pixels, expected %d\n", temp, PixelWidth*PixelHeight);
			if(feof(PixmapBin)) fprintf(stderr, "Cause : end of file reached\n");
			int err = ferror(PixmapBin);
			if(err){
				fprintf(stderr, "Cause : error %d\n", err);
				clearerr(PixmapBin);
			}
			free(PixelInt);
			exit(EXIT_FAILURE);
		}
		if(fclose(PixmapBin)) perror("Error : couldn't close Pixmap.bin");
	/*Find the balls*/
		struct coordinate Red = {-1, -1, -1};
		struct coordinate *PRed = &Red;
		FindBall(PixelInt, PixelWidth, PRed, TableMax, TableMin, BallDiameter, RBallMin, RBallMax);
		struct coordinate Yellow = {-1, -1, -1};
		struct coordinate *PYellow = &Yellow;
		FindBall(PixelInt, PixelWidth, PYellow, TableMax, TableMin, BallDiameter, YBallMin, YBallMax);
		struct coordinate White = {-1, -1, -1};
		struct coordinate *PWhite = &White;
		FindBall(PixelInt, PixelWidth, PWhite, TableMax, TableMin, BallDiameter, WBallMin, WBallMax);
		free(PixelInt);
	/*Check if the balls are all here*/
		if(Red.X < 0){
			fprintf(stderr, "Error : red ball missing\n");
			exit(EXIT_FAILURE);
		}
		if(Yellow.X < 0){
			fprintf(stderr, "Error : yellow ball missing\n");
			exit(EXIT_FAILURE);
		}
		if(White.X < 0){
			fprintf(stderr, "Error : white ball missing\n");
			exit(EXIT_FAILURE);
		}
	/*Open and write in Pos.txt*/
		FILE *PosTxt;
		PosTxt = fopen("Pos.txt", "w");
		if(PosTxt == NULL){
			perror("Error : couldn't open Pos.txt");
			exit(EXIT_FAILURE);
		}
		if(1 > fprintf(PosTxt, "Red:%d,%d,%d\nYellow:%d,%d,%d\nWhite:%d,%d,%d", Red.X, Red.Y, Red.Score, Yellow.X, Yellow.Y, Yellow.Score, White.X, White.Y, White.Score)){
			fprintf(stderr, "Error : couldn't write in Pos.txt\n");
			int err = ferror(PosTxt);
			if(err){
				fprintf(stderr, "Cause: error %d\n", err);
				clearerr(PosTxt);
			}
		}
		if(fclose(PosTxt)) perror("Error: couldn't close Pos.txt");

	exit(EXIT_SUCCESS);
}








