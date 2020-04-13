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

struct colourRange {
	struct colour Min;
	struct colour Max;
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
int GetScore(unsigned int *PixelInt, int PixelWidth, struct coordinate Coordinates, int DeltaX, int DeltaY, struct colourRange Range, short int Mode){
	int Score = 0;
	if(Mode == 2){
		struct colour PixelColour = Int2Colour(PixelInt[(Coordinates.X + DeltaX/4) + (Coordinates.Y + DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + DeltaX/4)+(Coordinates.Y + DeltaY/4)*PixelWidth);
		if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R && PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G && PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) Score++;
		PixelColour = Int2Colour(PixelInt[(Coordinates.X + 3*DeltaX/4) + (Coordinates.Y + DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + 3*DeltaX/4)+(Coordinates.Y + DeltaY/4)*PixelWidth);
		if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R && PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G && PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) Score++;
		PixelColour = Int2Colour(PixelInt[(Coordinates.X + DeltaX/4) + (Coordinates.Y + 3*DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + DeltaX/4)+(Coordinates.Y + 3*DeltaY/4)*PixelWidth);
		if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R && PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G && PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) Score++;
		PixelColour = Int2Colour(PixelInt[(Coordinates.X + 3*DeltaX/4) + (Coordinates.Y + 3*DeltaY/4)*PixelWidth]);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", (Coordinates.X + 3*DeltaX/4)+(Coordinates.Y + 3*DeltaY/4)*PixelWidth);
		if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R && PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G && PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) Score++;
	}
	else if(Mode == 1){
		int x = Coordinates.X + DeltaX/2;
		for(int y = Coordinates.Y; y < Coordinates.Y + DeltaY; y++){
			struct colour PixelColour = Int2Colour(PixelInt[x + y*PixelWidth]);
			if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", x + y*PixelWidth);
			if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R && PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G && PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) Score++;
		}
		int y = Coordinates.Y + DeltaY/2;
		for(int x = Coordinates.X; x < Coordinates.X + DeltaX; x++){
			struct colour PixelColour = Int2Colour(PixelInt[x + y*PixelWidth]);
			if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", x + y*PixelWidth);
			if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R && PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G && PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) Score++;
		}
	}
	else{
		for(int x = Coordinates.X; x < Coordinates.X + DeltaX; x++){
			for(int y = Coordinates.Y; y < Coordinates.Y + DeltaY; y++){
				struct colour PixelColour = Int2Colour(PixelInt[x + y*PixelWidth]);
				if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", x + y*PixelWidth);
				if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R && PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G && PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) Score++;
			}
		}
	}
	return Score;
}
	/*Converging to a better position*/
void Converge(unsigned int *PixelInt, int PixelWidth, struct coordinate *PCoordinate, int BallDiameter, struct colourRange Range){
	PCoordinate->X++;
	int tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->X-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->X++;
	PCoordinate->Y++;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->Y-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 1);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->Y++;
	PCoordinate->X++;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->X-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->X++;
	PCoordinate->Y++;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->Y-=2;
	tempscore = GetScore(PixelInt, PixelWidth, *(PCoordinate), BallDiameter, BallDiameter, Range, 0);
	if(tempscore > PCoordinate->Score){
		PCoordinate->Score = tempscore;
		Converge(PixelInt, PixelWidth, PCoordinate, BallDiameter, Range);
		return;
	}
	PCoordinate->Y++;
}
	/*Finding the balls*/
void FindBall(unsigned int *PixelInt, int PixelWidth, struct coordinate *PBall, struct coordinate TableMax, struct coordinate TableMin, short signed int BallDiameter, struct colourRange Range){
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
			if(GetScore(PixelInt, PixelWidth, Tile, BallDiameter, BallDiameter, Range, 2)){
				if(GetScore(PixelInt, PixelWidth, Tile, BallDiameter, BallDiameter, Range, 0) > BallDiameter*BallDiameter/5){
					struct coordinate *PTile = &Tile;
					PTile->Score = GetScore(PixelInt, PixelWidth, Tile, BallDiameter, BallDiameter, Range, 1);
					Converge(PixelInt, PixelWidth, PTile, BallDiameter, Range);
					if(Tile.Score > 7*BallDiameter*BallDiameter/10){
						if(PBall->X < 0){
							PBall->X = Tile.X;
							PBall->Y = Tile.Y;
							PBall->Score = Tile.Score;
						}
						else if(abs(PBall->X - Tile.X) > BallDiameter/2 || abs(PBall->Y - Tile.Y) > BallDiameter/2){
							fprintf(stderr, "Error : multiple balls in range: %d,%d,%d : %d,%d,%d, cannot continue\n", Range.Min.R, Range.Min.G, Range.Min.B, Range.Max.R,Range.Max.G,Range.Max.B);
							//fprintf(stderr, "Ball1: %d,%d:%d; Ball2: %d,%d:%d", PBall->X, PBall->Y, PBall->Score, Tile.X, Tile.Y, Tile.Score);		
							free(PixelInt);
							exit(EXIT_FAILURE);
						}
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
		struct colourRange RBall;
		struct colourRange YBall;
		struct colourRange WBall;
		struct colourRange BG;
		short signed int BallDiameter;
	/*Reads arguments*/
		if(argc == 30){
			TableMin.Y   = atoi(argv[1]);
			TableMax.Y   = atoi(argv[2]);
			TableMin.X   = atoi(argv[3]);
			TableMax.X   = atoi(argv[4]);
			RBall.Min.R   = atoi(argv[5]);
			RBall.Max.R   = atoi(argv[6]);
			RBall.Min.G   = atoi(argv[7]);
			RBall.Max.G   = atoi(argv[8]);
			RBall.Min.B   = atoi(argv[9]);
			RBall.Max.B   = atoi(argv[10]);
			YBall.Min.R   = atoi(argv[11]);
			YBall.Max.R   = atoi(argv[12]);
			YBall.Min.G   = atoi(argv[13]);
			YBall.Max.G   = atoi(argv[14]);
			YBall.Min.B   = atoi(argv[15]);
			YBall.Max.B   = atoi(argv[16]);
			WBall.Min.R   = atoi(argv[17]);
			WBall.Max.R   = atoi(argv[18]);
			WBall.Min.G   = atoi(argv[19]);
			WBall.Max.G   = atoi(argv[20]);
			WBall.Min.B   = atoi(argv[21]);
			WBall.Max.B   = atoi(argv[22]);
			BG.Min.R      = atoi(argv[23]);
			BG.Max.R      = atoi(argv[24]);
			BG.Min.G      = atoi(argv[25]);
			BG.Max.G      = atoi(argv[26]);
			BG.Min.B      = atoi(argv[27]);
			BG.Max.B      = atoi(argv[28]);
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
		if(RBall.Min.R > RBall.Max.R || RBall.Min.G > RBall.Max.G || RBall.Min.B > RBall.Max.B || RBall.Min.R < 0 || RBall.Min.G < 0 || RBall.Min.B < 0){
			fprintf(stderr, "Error : invalid values passed as red ball colour range, cannot continue\n");
			ErrorFlag = 1;
		}
		if(YBall.Min.R > YBall.Max.R || YBall.Min.G > YBall.Max.G || YBall.Min.B > YBall.Max.B || YBall.Min.R < 0 || YBall.Min.G < 0 || YBall.Min.B < 0){
			fprintf(stderr, "Error : invalid values passed as yellow ball colour range, cannot continue\n");
			ErrorFlag = 1;
		}
		if(WBall.Min.R > WBall.Max.R || WBall.Min.G > WBall.Max.G || WBall.Min.B > WBall.Max.B || WBall.Min.R < 0 || WBall.Min.G < 0 || WBall.Min.B < 0){
			fprintf(stderr, "Error : invalid values passed as white ball colour range, cannot continue\n");
			ErrorFlag = 1;
		}
		if(BG.Min.R > BG.Max.R || BG.Min.G > BG.Max.G || BG.Min.B > BG.Max.B || BG.Min.R < 0 || BG.Min.G < 0 || BG.Min.B < 0){
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
		FindBall(PixelInt, PixelWidth, PRed, TableMax, TableMin, BallDiameter, RBall);
		struct coordinate Yellow = {-1, -1, -1};
		struct coordinate *PYellow = &Yellow;
		FindBall(PixelInt, PixelWidth, PYellow, TableMax, TableMin, BallDiameter, YBall);
		struct coordinate White = {-1, -1, -1};
		struct coordinate *PWhite = &White;
		FindBall(PixelInt, PixelWidth, PWhite, TableMax, TableMin, BallDiameter, WBall);
		free(PixelInt);
	/*Check if the balls are all here and if they are overlapping*/
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
		if(abs(Red.X - White.X) < BallDiameter && abs(Red.Y - White.Y) < BallDiameter){
			fprintf(stderr, "Error : white ball and red ball overlapping\n");
			exit(EXIT_FAILURE);
		}
		if(abs(Red.X - Yellow.X) < BallDiameter && abs(Red.Y - Yellow.Y) < BallDiameter){
			fprintf(stderr, "Error : yellow ball and red ball overlapping\n");
			exit(EXIT_FAILURE);
		}
		if(abs(Yellow.X - White.X) < BallDiameter && abs(Yellow.Y - White.Y) < BallDiameter){
			fprintf(stderr, "Error : white ball and yellow ball overlapping\n");
			exit(EXIT_FAILURE);
		}
	/*Open and write in Pos.txt*/
		FILE *PosTxt;
		PosTxt = fopen("Pos.txt", "w");
		if(PosTxt == NULL){
			perror("Error : couldn't open Pos.txt");
			exit(EXIT_FAILURE);
		}
		if(1 > fprintf(PosTxt, "Red: %d, %d, %d\nYellow: %d, %d, %d\nWhite: %d, %d, %d", Red.X, Red.Y, Red.Score, Yellow.X, Yellow.Y, Yellow.Score, White.X, White.Y, White.Score)){
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