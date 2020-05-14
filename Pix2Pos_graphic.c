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

const char err_eof[] = "Cause : end of file reached\n";
const char err_cause[] = "Cause : error %d\n";
const char err_color[] = "Error : colour error at pixel %d ignoring pixel\n";
const char err_numarg[] = "Error : invalid number of argument, cannot continue\n";
const char err_tabsize[] = "Error : invalid values passed as table size, cannot continue\n";
const char err_redrange[] = "Error : invalid values passed as red ball colour range, cannot continue\n";
const char err_yelrange[] = "Error : invalid values passed as yellow ball colour range, cannot continue\n";
const char err_whirange[] = "Error : invalid values passed as white ball colour range, cannot continue\n";
const char err_bgrange[] = "Error : invalid values passed as background colour range, cannot continue\n";
const char err_baltsize[] = "Error : invalid values passed as table and ball size, ball is bigger than table, cannot continue\n";
const char err_ballsize[] = "Error : invalid values passed as ball size, cannot continue\n";
const char err_open[] = "Error : couldn't open Pixmap.bin";
const char err_imsize[] = "Error : invalid values passed as image size, cannot continue\n";

/*Structure declaration*/struct colour {short int R;short int G;short int B;};struct colourRange {struct colour Min;struct colour Max;};struct coordinate {short int X;short int Y;short int Score;};struct coordinateRange {struct coordinate Min;struct coordinate Max;
};struct pixmap {unsigned int Width;unsigned int Height;unsigned int *Pixmap;};/*Function declaration*/int readFile(unsigned int *ptr, int size, int amount, int addition, FILE *file){int temp = fread(ptr, size, amount+addition, file);if(temp != amount){fprintf(stderr, "Read %d elements, expected %d\n", temp, amount);
			if(feof(file)) fprintf(stderr, err_eof);
			int err = ferror(file);
			if(err){
				fprintf(stderr, err_cause, err);
				clearerr(file);
			}
			return -1;
		}
		return 0;
	}
	struct coordinateRange buildNeighbourhood(struct coordinate Ball, struct coordinateRange Table, int size, int BallDiameter){
		struct coordinateRange Neighbourhood;
		Neighbourhood.Min.X = (Ball.X - size) < Table.Min.X ? Table.Min.X : Ball.X - size;
		Neighbourhood.Max.X = (Ball.X + size + BallDiameter) > Table.Max.X ? Table.Max.X : Ball.X + size + BallDiameter;
		Neighbourhood.Min.Y = (Ball.Y - size) < Table.Min.Y ? Table.Min.Y : Ball.Y - size;
		Neighbourhood.Max.Y = (Ball.Y + size + BallDiameter) > Table.Max.Y ? Table.Max.Y : Ball.Y + size + BallDiameter;
		return Neighbourhood;
	}	
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
	int CheckColour(int pixel, int index, struct colourRange Range){
		struct colour PixelColour = Int2Colour(pixel);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, err_color, index);
		if(PixelColour.R >= Range.Min.R && PixelColour.R <= Range.Max.R 
			&& PixelColour.G >= Range.Min.G && PixelColour.G <= Range.Max.G 
			&& PixelColour.B >= Range.Min.B && PixelColour.B <= Range.Max.B) 
				return 1;
		else return 0;
	}
	int GetScore(struct pixmap Pixels, struct coordinate Coordinates, int Delta, struct colourRange Range, short int Mode){
		int Score = 0;
		if(Mode == 2){
			int indexes[16] = {
				(Coordinates.X + Delta/8)+(Coordinates.Y + Delta/8)*Pixels.Width,
				(Coordinates.X + 3*Delta/8)+(Coordinates.Y + Delta/8)*Pixels.Width,
				(Coordinates.X + 5*Delta/8)+(Coordinates.Y + Delta/8)*Pixels.Width,
				(Coordinates.X + 7*Delta/8)+(Coordinates.Y + Delta/8)*Pixels.Width,
				(Coordinates.X + Delta/8)+(Coordinates.Y + 3*Delta/8)*Pixels.Width,
				(Coordinates.X + 3*Delta/8)+(Coordinates.Y + 3*Delta/8)*Pixels.Width,
				(Coordinates.X + 5*Delta/8)+(Coordinates.Y + 3*Delta/8)*Pixels.Width,
				(Coordinates.X + 7*Delta/8)+(Coordinates.Y + 3*Delta/8)*Pixels.Width,
				(Coordinates.X + Delta/8)+(Coordinates.Y + 5*Delta/8)*Pixels.Width,
				(Coordinates.X + 3*Delta/8)+(Coordinates.Y + 5*Delta/8)*Pixels.Width,
				(Coordinates.X + 5*Delta/8)+(Coordinates.Y + 5*Delta/8)*Pixels.Width,
				(Coordinates.X + 7*Delta/8)+(Coordinates.Y + 5*Delta/8)*Pixels.Width,
				(Coordinates.X + Delta/8)+(Coordinates.Y + 7*Delta/8)*Pixels.Width,
				(Coordinates.X + 3*Delta/8)+(Coordinates.Y + 7*Delta/8)*Pixels.Width,
				(Coordinates.X + 5*Delta/8)+(Coordinates.Y + 7*Delta/8)*Pixels.Width,
				(Coordinates.X + 7*Delta/8)+(Coordinates.Y + 7*Delta/8)*Pixels.Width,
			};
			for(int i = 0; i < 16; i++){
				Score += CheckColour(Pixels.Pixmap[indexes[i]], indexes[i], Range);
			}
		}
		else if(Mode == 1){
			int x = Coordinates.X + Delta/2;
			for(int y = Coordinates.Y; y < Coordinates.Y + Delta; y++){
				Score += CheckColour(Pixels.Pixmap[x + y*Pixels.Width], x + y*Pixels.Width, Range);
			}
			int y = Coordinates.Y + Delta/2;
			for(int x = Coordinates.X; x < Coordinates.X + Delta; x++){
				Score += CheckColour(Pixels.Pixmap[x + y*Pixels.Width], x + y*Pixels.Width, Range);
			}
		}
		else{
			for(int x = Coordinates.X; x < Coordinates.X + Delta; x++){
				for(int y = Coordinates.Y; y < Coordinates.Y + Delta; y++){
					Score += CheckColour(Pixels.Pixmap[x + y*Pixels.Width], x + y*Pixels.Width, Range);
				}
			}
		}
		return Score;
	}
	void Converge(struct pixmap Pixels, struct coordinate *PCoordinate, int BallDiameter, struct colourRange Range){
		struct coordinate TempCoords[4] = {
			{PCoordinate->X + 1, PCoordinate->Y, 0},
			{PCoordinate->X, PCoordinate->Y + 1, 0},
			{PCoordinate->X - 1, PCoordinate->Y, 0},
			{PCoordinate->X, PCoordinate->Y - 1, 0},
		};
		for(int level = 1; level >= 0; level--){
			for(int i = 0; i < 4; i++){
				TempCoords[i].Score = GetScore(Pixels, TempCoords[i], BallDiameter, Range, level);
				if(TempCoords[i].Score > PCoordinate->Score){
					*PCoordinate = TempCoords[i];
					Converge(Pixels, PCoordinate, BallDiameter, Range);
					return;
				}
			}
		}
	}
	void FindBall(struct pixmap Pixels, struct coordinate *PBall, struct coordinateRange Table, short int BallDiameter, struct colourRange Range){
		struct coordinate TileAmount;
		TileAmount.X = (Table.Max.X-Table.Min.X) / BallDiameter + ((Table.Max.X-Table.Min.X) % BallDiameter == 0 ? 0: 1);
		TileAmount.Y = (Table.Max.Y-Table.Min.Y) / BallDiameter + ((Table.Max.Y-Table.Min.Y) % BallDiameter == 0 ? 0: 1);
		for(int TileY = 0; TileY < TileAmount.Y; TileY++){
			int y = Table.Min.Y + TileY*BallDiameter;
			if(y+BallDiameter > Table.Max.Y) y -= y + BallDiameter - Table.Max.Y;
			for(int TileX = 0; TileX < TileAmount.X; TileX++){
				int x = Table.Min.X + TileX*BallDiameter;
				if(x+BallDiameter > Table.Max.X) x -= x + BallDiameter - Table.Max.X;
				struct coordinate Tile = {x, y, 0};
				if(GetScore(Pixels, Tile, BallDiameter, Range, 2)){
					struct coordinate *PTile = &Tile;
					PTile->Score = GetScore(Pixels, Tile, BallDiameter, Range, 1);
					Converge(Pixels, PTile, BallDiameter, Range);
					if(Tile.Score > PBall->Score){
						PBall->X = Tile.X;
						PBall->Y = Tile.Y;
						PBall->Score = Tile.Score;
					}
				}
			}
		}
	}




/*Main*/
int main(int argc, char **argv){	
	/*Variable Declaration*/
		struct coordinateRange Table;
		struct colourRange RBall;
		struct colourRange YBall;
		struct colourRange WBall;
		struct colourRange BG;
		short int BallDiameter;
		struct pixmap Pixels;
		struct coordinate Red = {-1, -1, -1};
		struct coordinate *PRed = &Red;
		struct coordinate Yellow = {-1, -1, -1};
		struct coordinate *PYellow = &Yellow;
		struct coordinate White = {-1, -1, -1};
		struct coordinate *PWhite = &White;
	/*Read arguments*/
		if(argc == 30){
			Table.Min.Y  = atoi(argv[1]);
			Table.Max.Y  = atoi(argv[2]);
			Table.Min.X  = atoi(argv[3]);
			Table.Max.X  = atoi(argv[4]);
			RBall.Min.R  = atoi(argv[5]);
			RBall.Max.R  = atoi(argv[6]);
			RBall.Min.G  = atoi(argv[7]);
			RBall.Max.G  = atoi(argv[8]);
			RBall.Min.B  = atoi(argv[9]);
			RBall.Max.B  = atoi(argv[10]);
			YBall.Min.R  = atoi(argv[11]);
			YBall.Max.R  = atoi(argv[12]);
			YBall.Min.G  = atoi(argv[13]);
			YBall.Max.G  = atoi(argv[14]);
			YBall.Min.B  = atoi(argv[15]);
			YBall.Max.B  = atoi(argv[16]);
			WBall.Min.R  = atoi(argv[17]);
			WBall.Max.R  = atoi(argv[18]);
			WBall.Min.G  = atoi(argv[19]);
			WBall.Max.G  = atoi(argv[20]);
			WBall.Min.B  = atoi(argv[21]);
			WBall.Max.B  = atoi(argv[22]);
			BG.Min.R     = atoi(argv[23]);
			BG.Max.R     = atoi(argv[24]);
			BG.Min.G     = atoi(argv[25]);
			BG.Max.G     = atoi(argv[26]);
			BG.Min.B     = atoi(argv[27]);
			BG.Max.B     = atoi(argv[28]);
			BallDiameter = atoi(argv[29]);
		}
		else {
			fprintf(stderr, err_numarg);
			return -1;
		}
	
	
	/*Check first errors*/
		if(Table.Min.X > Table.Max.X || Table.Min.Y > Table.Max.Y || Table.Min.X < 0 || Table.Min.Y < 0){
			fprintf(stderr, err_tabsize);
			return -1;
		}
		if(RBall.Min.R > RBall.Max.R || RBall.Min.G > RBall.Max.G || RBall.Min.B > RBall.Max.B || RBall.Min.R < 0 || RBall.Min.G < 0 || RBall.Min.B < 0){
			fprintf(stderr, err_redrange);
			return -1;
		}
		if(YBall.Min.R > YBall.Max.R || YBall.Min.G > YBall.Max.G || YBall.Min.B > YBall.Max.B || YBall.Min.R < 0 || YBall.Min.G < 0 || YBall.Min.B < 0){
			fprintf(stderr, err_yelrange);
			return -1;
		}
		if(WBall.Min.R > WBall.Max.R || WBall.Min.G > WBall.Max.G || WBall.Min.B > WBall.Max.B || WBall.Min.R < 0 || WBall.Min.G < 0 || WBall.Min.B < 0){
			fprintf(stderr, err_whirange);
			return -1;
		}
		if(BG.Min.R > BG.Max.R || BG.Min.G > BG.Max.G || BG.Min.B > BG.Max.B || BG.Min.R < 0 || BG.Min.G < 0 || BG.Min.B < 0){
			fprintf(stderr, err_bgrange);
			return -1;
		}
		if(Table.Max.X - Table.Min.X < BallDiameter || Table.Max.Y - Table.Min.Y < BallDiameter){
			fprintf(stderr, err_baltsize);
			return -1;
		}
		if(BallDiameter < MIN_BALL_DIAMETER || BallDiameter > MAX_BALL_DIAMETER){
			fprintf(stderr, err_ballsize);
			return -1;
		}
	/*Open Pixmap.bin and verify width and heigth*/
		FILE *PixmapBin;
		PixmapBin = fopen("Pixmap.bin", "rb");
		if(PixmapBin == NULL){
			perror(err_open);
			return -1;
		}
		if(readFile(&(Pixels.Width), sizeof(unsigned int), 1, 0, PixmapBin)) return -1;
		if(readFile(&(Pixels.Height), sizeof(unsigned int), 1, 0, PixmapBin)) return -1;
		if(Pixels.Width < MIN_IMAGE_WIDTH || Pixels.Height < MIN_IMAGE_HEIGHT || Pixels.Width > MAX_IMAGE_WIDTH || Pixels.Height > MAX_IMAGE_HEIGHT){
			fprintf(stderr, err_imsize);
			return -1;
		}
	/*Read the pixels and close the file*/
		Pixels.Pixmap = malloc(sizeof(unsigned int)*(Pixels.Width*Pixels.Height+1));
		if(readFile(Pixels.Pixmap, sizeof(unsigned int), Pixels.Width*Pixels.Height, 1, PixmapBin)){
			free(Pixels.Pixmap);
			return -1;
		}

const char err_close[] = "Error : couldn't close Pixmap.bin";
const char err_nored[] = "Error : red ball missing\n";
const char err_noyel[] = "";
const char err_nowhi[] = "";

		if(fclose(PixmapBin)) perror(err_close);
	/*Find the balls*/
		FILE *PosTxt;
		PosTxt = fopen("Pos.txt", "r");
		if(PosTxt != NULL){
			struct coordinate Temp = {-1, -1, -1};
			if(3 == fscanf(PosTxt, "Red: %hd, %hd, %hd\n", &(Temp.X), &(Temp.Y), &(Temp.Score))){
				struct coordinateRange Neighbourhood = buildNeighbourhood(Temp, Table, 2*BallDiameter, BallDiameter);
				FindBall(Pixels, PRed, Neighbourhood, BallDiameter, RBall);
			}
			if(3 == fscanf(PosTxt, "Yellow: %hd, %hd, %hd\n", &(Temp.X), &(Temp.Y), &(Temp.Score))){
				struct coordinateRange Neighbourhood = buildNeighbourhood(Temp, Table, 2*BallDiameter, BallDiameter);
				FindBall(Pixels, PYellow, Neighbourhood, BallDiameter, YBall);
			}
			if(3 == fscanf(PosTxt, "White: %hd, %hd, %hd\n", &(Temp.X), &(Temp.Y), &(Temp.Score))){
				struct coordinateRange Neighbourhood = buildNeighbourhood(Temp, Table, 2*BallDiameter, BallDiameter);
				FindBall(Pixels, PWhite, Neighbourhood, BallDiameter, WBall);
			}
		}
		if(Red.Score < 7*BallDiameter*BallDiameter/10) FindBall(Pixels, PRed, Table, BallDiameter, RBall);
		if(Yellow.Score < 7*BallDiameter*BallDiameter/10) FindBall(Pixels, PYellow, Table, BallDiameter, YBall);
		if(White.Score < 7*BallDiameter*BallDiameter/10) FindBall(Pixels, PWhite, Table, BallDiameter, WBall);
		free(Pixels.Pixmap);
	/*Check if the balls are all here and if they are overlapping*/
		if(Red.Score < 0){
			fprintf(stderr, err_nored);
			return -1;
		}
		if(Yellow.Score < 0){
			fprintf(stderr, err_nored);
			return -1;
		}
		if(White.Score < 0){
			fprintf(stderr, "Error : white ball missing\n");
			return -1;
		}
		if(abs(Red.X - White.X) <= BallDiameter && abs(Red.Y - White.Y) <= BallDiameter){
			fprintf(stderr, "Error : white ball and red ball overlapping\n");
			return -1;
		}
		if(abs(Red.X - Yellow.X) <= BallDiameter && abs(Red.Y - Yellow.Y) <= BallDiameter){
			fprintf(stderr, "Error : yellow ball and red ball overlapping\n");
			return -1;
		}
		if(abs(Yellow.X - White.X) <= BallDiameter && abs(Yellow.Y - White.Y) <= BallDiameter){
			fprintf(stderr, "Error : white ball and yellow ball overlapping\n");
			return -1;
		}
	/*Open and write in Pos.txt*/
		PosTxt = fopen("Pos.txt", "w");
		if(PosTxt == NULL){
			perror("Error : couldn't open Pos.txt");
			return -1;
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

	return 0;
}
