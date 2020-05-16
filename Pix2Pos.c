/*
 *	File:				Pix2Pos.c
 *
 *	Description: 		Billiard configuration analysis: Balls finder
 *	
 *	Full Description: 	Reads a Pixmap.bin file. 
 *						Reads the image size and the image pixels. 
 *						Separates the image in tiles.
 *						Makes the interesting tiles converge to the balls. 
 *						Stores the ball coordinates in Pos.txt
 *
 *	Dependancy:			Pixmap.bin
 *
 *	Authors:			Dufour Edouard; Rochet Corentin		No copyright
 *
 *	Version:			3.2.1
 *
 *	Revision:			16.05.2020, ED, PrettyPrint and final revision
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BALL_DIAMETER 5
#define MAX_BALL_DIAMETER 20
#define MIN_IMAGE_WIDTH 10
#define MIN_IMAGE_HEIGHT 10
#define MAX_IMAGE_WIDTH 1000
#define MAX_IMAGE_HEIGHT 1000


/*Structure declaration*/
	typedef struct colour {int R, G, B;} colour;

	typedef struct colourRange {struct colour Min, Max;} colourRange;

	typedef struct coordinate {int X, Y, Score;} coordinate;

	typedef struct coordinateRange {struct coordinate Min, Max;} coordinateRange;

	typedef struct pixmap {unsigned int Width, Height, *Pixmap;} pixmap;



/*Function declaration*/
	int readCommandLine(int argc, char **argv, coordinateRange *Table, colourRange *RBall, colourRange *YBall, colourRange *WBall, colourRange *BG, int *BallDiameter);

	int readFile(unsigned int *ptr, int size, int amount, _Bool addition, FILE *file);

	coordinateRange buildNeighbourhood(coordinate *Center, coordinateRange *Limits, int size, int Offset);

	colour Int2Colour(int ColourInt);

	int CheckColour(int pixel, int index, colourRange *Range);

	int GetScore(pixmap *Pixels, coordinate *Coordinates, int Delta, colourRange *Range, int Mode);

	void Converge(pixmap *Pixels, coordinate *PCoordinate, int SquareSize, colourRange *Range);

	void FindBall(pixmap *Pixels, coordinate *PBall, coordinateRange *Table, int BallDiameter, colourRange *Range);




	



/*Main*/
int main(int argc, char **argv){	
	/*Variable Declaration*/
		coordinateRange Table;
		colourRange RBall;
		colourRange YBall;
		colourRange WBall;
		colourRange BG;
		int BallDiameter;
		pixmap Pixels;
		coordinate Red = {-1, -1, -1};
		coordinate Yellow = {-1, -1, -1};
		coordinate White = {-1, -1, -1};
	/*Read arguments*/
		if(readCommandLine(argc, argv, &Table, &RBall, &YBall, &WBall, &BG, &BallDiameter)) return -1;
	/*Open Pixmap.bin and verify width and heigth*/
		FILE *PixmapBin;
		PixmapBin = fopen("Pixmap.bin", "rb");
		if(PixmapBin == NULL){
			perror("Error : couldn't open Pixmap.bin");
			return -1;
		}
		if(readFile(&(Pixels.Width), sizeof(unsigned int), 1, 0, PixmapBin)) return -1;
		if(readFile(&(Pixels.Height), sizeof(unsigned int), 1, 0, PixmapBin)) return -1;
		if(Pixels.Width < MIN_IMAGE_WIDTH || Pixels.Height < MIN_IMAGE_HEIGHT || Pixels.Width > MAX_IMAGE_WIDTH || Pixels.Height > MAX_IMAGE_HEIGHT){
			fprintf(stderr, "Error : invalid values passed as image size, cannot continue\n");
			return -1;
		}
	/*Read the pixels and close the file*/
		Pixels.Pixmap = malloc(sizeof(unsigned int)*(Pixels.Width*Pixels.Height+1));
		if(readFile(Pixels.Pixmap, sizeof(unsigned int), Pixels.Width*Pixels.Height, 1, PixmapBin)){
			free(Pixels.Pixmap);
			if(fclose(PixmapBin)) perror("Error : couldn't close Pixmap.bin");
			return -1;
		}
		if(fclose(PixmapBin)) perror("Error : couldn't close Pixmap.bin");
	/*Read Pos.txt and try to find the balls in the neighourhood of the old balls*/
		FILE *PosTxt;
		PosTxt = fopen("Pos.txt", "r");
		if(PosTxt != NULL){
			coordinate Temp = {-1, -1, -1};
			if(3 == fscanf(PosTxt, "Red: %d, %d, %d\n", &(Temp.X), &(Temp.Y), &(Temp.Score))){
				coordinateRange Neighbourhood = buildNeighbourhood(&Temp, &Table, 2*BallDiameter, BallDiameter);
				FindBall(&Pixels, &Red, &Neighbourhood, BallDiameter, &RBall);
			}
			if(3 == fscanf(PosTxt, "Yellow: %d, %d, %d\n", &(Temp.X), &(Temp.Y), &(Temp.Score))){
				coordinateRange Neighbourhood = buildNeighbourhood(&Temp, &Table, 2*BallDiameter, BallDiameter);
				FindBall(&Pixels, &Yellow, &Neighbourhood, BallDiameter, &YBall);
			}
			if(3 == fscanf(PosTxt, "White: %d, %d, %d\n", &(Temp.X), &(Temp.Y), &(Temp.Score))){
				coordinateRange Neighbourhood = buildNeighbourhood(&Temp, &Table, 2*BallDiameter, BallDiameter);
				FindBall(&Pixels, &White, &Neighbourhood, BallDiameter, &WBall);
			}
			if(fclose(PosTxt)) perror("Error: couldn't close Pos.txt");
		}
	/*If the balls are not yet found, try to find them on the whole table*/
		if(Red.Score < 7*BallDiameter*BallDiameter/10) FindBall(&Pixels, &Red, &Table, BallDiameter, &RBall);
		if(Yellow.Score < 7*BallDiameter*BallDiameter/10) FindBall(&Pixels, &Yellow, &Table, BallDiameter, &YBall);
		if(White.Score < 7*BallDiameter*BallDiameter/10) FindBall(&Pixels, &White, &Table, BallDiameter, &WBall);
		free(Pixels.Pixmap);
	/*Check if the balls are all here and if they are overlapping*/
		if(Red.Score < 0){
			fprintf(stderr, "Error : red ball missing\n");
			return -1;
		}
		if(Yellow.Score < 0){
			fprintf(stderr, "Error : yellow ball missing\n");
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





/*Function initialization*/
	__inline int readCommandLine(int argc, char **argv, coordinateRange *Table, colourRange *RBall, colourRange *YBall, colourRange *WBall, colourRange *BG, int *BallDiameter){
		/*
		 *	Name:				readCommandLine
		 *
		 *	Description:		Reads the command line, sets the main variables and checks their validity.
		 *
		 *	Inputs:
		 *		argc/argv:		Command line
		 *		Table:			Table coordinates
		 *		RBall, YBall, WBall:	Colour ranges of the balls
		 *		BG:				Colour range of the background (unused)
		 *		BallDiameter:	Size of the ball
		 *	Return:			
		 *		return:			0 if all worked perfectly, else -1
		 */
		if(argc == 30){
			Table->Min.Y  = atoi(argv[1]);
			Table->Max.Y  = atoi(argv[2]);
			Table->Min.X  = atoi(argv[3]);
			Table->Max.X  = atoi(argv[4]);
			RBall->Min.R  = atoi(argv[5]);
			RBall->Max.R  = atoi(argv[6]);
			RBall->Min.G  = atoi(argv[7]);
			RBall->Max.G  = atoi(argv[8]);
			RBall->Min.B  = atoi(argv[9]);
			RBall->Max.B  = atoi(argv[10]);
			YBall->Min.R  = atoi(argv[11]);
			YBall->Max.R  = atoi(argv[12]);
			YBall->Min.G  = atoi(argv[13]);
			YBall->Max.G  = atoi(argv[14]);
			YBall->Min.B  = atoi(argv[15]);
			YBall->Max.B  = atoi(argv[16]);
			WBall->Min.R  = atoi(argv[17]);
			WBall->Max.R  = atoi(argv[18]);
			WBall->Min.G  = atoi(argv[19]);
			WBall->Max.G  = atoi(argv[20]);
			WBall->Min.B  = atoi(argv[21]);
			WBall->Max.B  = atoi(argv[22]);
			BG->Min.R     = atoi(argv[23]);
			BG->Max.R     = atoi(argv[24]);
			BG->Min.G     = atoi(argv[25]);
			BG->Max.G     = atoi(argv[26]);
			BG->Min.B     = atoi(argv[27]);
			BG->Max.B     = atoi(argv[28]);
			*BallDiameter = atoi(argv[29]);
		}
		else {
			fprintf(stderr, "Error : invalid number of argument, cannot continue\n");
			return -1;
		}
		if(Table->Min.X < 0 || Table->Min.Y < 0 || Table->Min.X > Table->Max.X || Table->Min.Y > Table->Max.Y){
			fprintf(stderr, "Error : invalid values passed as table size, cannot continue\n");
			return -1;
		}
		if(RBall->Min.R < 0 || RBall->Min.G < 0 || RBall->Min.B < 0 || RBall->Min.R > RBall->Max.R || RBall->Min.G > RBall->Max.G || RBall->Min.B > RBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as red ball colour range, cannot continue\n");
			return -1;
		}
		if(YBall->Min.R < 0 || YBall->Min.G < 0 || YBall->Min.B < 0 || YBall->Min.R > YBall->Max.R || YBall->Min.G > YBall->Max.G || YBall->Min.B > YBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as yellow ball colour range, cannot continue\n");
			return -1;
		}
		if(WBall->Min.R < 0 || WBall->Min.G < 0 || WBall->Min.B < 0 || WBall->Min.R > WBall->Max.R || WBall->Min.G > WBall->Max.G || WBall->Min.B > WBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as white ball colour range, cannot continue\n");
			return -1;
		}
		if(BG->Min.R < 0 || BG->Min.G < 0 || BG->Min.B < 0 || BG->Min.R > BG->Max.R || BG->Min.G > BG->Max.G || BG->Min.B > BG->Max.B){
			fprintf(stderr, "Error : invalid values passed as background colour range, cannot continue\n");
			return -1;
		}
		if(Table->Max.X - Table->Min.X < *BallDiameter || Table->Max.Y - Table->Min.Y < *BallDiameter){
			fprintf(stderr, "Error : invalid values passed as table and ball size, ball is bigger than table, cannot continue\n");
			return -1;
		}
		if(*BallDiameter < MIN_BALL_DIAMETER || *BallDiameter > MAX_BALL_DIAMETER){
			fprintf(stderr, "Error : invalid values passed as ball size, cannot continue\n");
			return -1;
		}
		return 0;
	}
	
	__inline int readFile(unsigned int *ptr, int size, int amount, _Bool addition, FILE *file){
		/*
		 *	Name:				readFile
		 *
		 *	Description:		Reads a certain amount of a given file and handles errors
		 *
		 *	Full Description:	Reads a given amount of elements of given size in a given file and stores the result in a pointer.
		 *						It handles errors such as: 	End of file reached
		 *													Not the right amount of elements read (too few or too many)
		 *						Prints the error code
		 *
		 *	Inputs:
		 *		*ptr:			Pointer to store the read data in
		 *		size:			Size of each element to read
		 *		amount:			Amount of elements to read
		 *		addition:		This variable is used to try to read an additional entry to detect if too many entries are in the file.
		 *		*file:			File to read from
		 *	Return:			
		 *		return:			0 if all worked perfectly, else -1
		 *	Errors:
		 *						Wrong amount of elements read
		 *						End of file reached
		 *						Errorcode
		 */
		int temp = fread(ptr, size, amount+addition, file);
		if(temp != amount){
			fprintf(stderr, "Read %d elements, expected %d\n", temp, amount);
			if(feof(file)) fprintf(stderr, "Cause : end of file reached\n");
			int err = ferror(file);
			if(err){
				fprintf(stderr, "Cause : error %d\n", err);
				clearerr(file);
			}
			return -1;
		}
		return 0;
	}

	__inline coordinateRange buildNeighbourhood(coordinate *Center, coordinateRange *Limits, int size, int Offset){
		/*
		 *	Name:				buildNeighbourhood
		 *
		 *	Description:		Builds a coordinate range of given size around a given point while staying in a given coordinate range
		 *
		 *	Inputs:
		 *		Center:			Center of the neighbourhood to build
		 *		Limits:			Limits the built neighbourhood shouldn't cross
		 *		size:			Size of the neighbourhood
		 *		Offset:			Offset of the center of the neighbourhood
		 *	Output:			
		 *		Neighbourhood:	Built neighbourhood
		 */
		coordinateRange Neighbourhood;
		Neighbourhood.Min.X = (Center->X - size) < Limits->Min.X ? Limits->Min.X : Center->X - size;
		Neighbourhood.Max.X = (Center->X + size + Offset) > Limits->Max.X ? Limits->Max.X : Center->X + size + Offset;
		Neighbourhood.Min.Y = (Center->Y - size) < Limits->Min.Y ? Limits->Min.Y : Center->Y - size;
		Neighbourhood.Max.Y = (Center->Y + size + Offset) > Limits->Max.Y ? Limits->Max.Y : Center->Y + size + Offset;
		return Neighbourhood;
	}

	__inline colour Int2Colour(int ColourInt) {
		/*
		 *	Name:				Int2Colour
		 *
		 *	Description:		Converts a colour integer to RGB
		 *
		 *	Inputs:
		 *		ColourInt:		Colour integer
		 *	Output:			
		 *		ColourRGB:		Colour RGB
		 */
		colour ColourRGB = {-1, -1, -1};
		/*Detecting if int not in correct range*/
			if(ColourInt < 0 || ColourInt > 16777215){
				return ColourRGB;
			}
		/*Calculating colour*/
			ColourRGB.R = (ColourInt & 0x00FF0000) >> 16;
			ColourRGB.G = (ColourInt & 0x0000FF00) >> 8;
			ColourRGB.B = (ColourInt & 0x000000FF);
		return ColourRGB;
	}

	__inline int CheckColour(int pixel, int index, colourRange *Range){
		/*
		 *	Name:				CheckColour
		 *
		 *	Description:		Checks if a given pixel is inside a given colour range
		 *
		 *	Inputs:
		 *		pixel:			Pixel to check
		 *		index:			Index of the given pixel
		 *		Range:			Colour range the pixel should be in
		 *	Return:			
		 *		return:			1 if the pixel is in the range, 0 else
		 *	Errors:
		 *						Pixel colour invalid
		 */
		colour PixelColour = Int2Colour(pixel);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", index);
		if(PixelColour.R >= Range->Min.R && PixelColour.R <= Range->Max.R 
			&& PixelColour.G >= Range->Min.G && PixelColour.G <= Range->Max.G 
			&& PixelColour.B >= Range->Min.B && PixelColour.B <= Range->Max.B) 
				return 1;
		else return 0;
	}

	int GetScore(pixmap *Pixels, coordinate *Coordinates, int Delta, colourRange *Range, int Mode){
		/*
		 *	Name:				GetScore
		 *
		 *	Description:		Gets the score of a given square
		 *
		 *	Full Description:	This function calculates the score of an amount of pixel given by the mode
		 *		Mode 0				All the pixels of the square
		 *		Mode 1				Central cross of the same size as the square
		 *		Mode 2				16 pixels uniformly distributed in the square
		 *
		 *	Inputs:
		 *		*ptr:			Pointer to store the read data in
		 *		size:			Size of each element to read
		 *		amount:			Amount of elements to read
		 *		addition:		This variable is used to try to read an additional entry to detect if too many entries are in the file.
		 *		*file:			File to read from
		 *	Return:			
		 *		return:			0 if all worked perfectly, else -1
		 *	Errors:
		 *						Wrong amount of elements read
		 *						End of file reached
		 *						Errorcode
		 */
		int Score = 0;
		if(Mode == 2){
			int indexes[16] = {
				//(Coordinates->X + Delta/8)+(Coordinates->Y + Delta/8)*Pixels->Width,
				//(Coordinates->X + 3*Delta/8)+(Coordinates->Y + Delta/8)*Pixels->Width,
				//(Coordinates->X + 5*Delta/8)+(Coordinates->Y + Delta/8)*Pixels->Width,
				//(Coordinates->X + 7*Delta/8)+(Coordinates->Y + Delta/8)*Pixels->Width,
				//(Coordinates->X + Delta/8)+(Coordinates->Y + 3*Delta/8)*Pixels->Width,
				//(Coordinates->X + 3*Delta/8)+(Coordinates->Y + 3*Delta/8)*Pixels->Width,
				//(Coordinates->X + 5*Delta/8)+(Coordinates->Y + 3*Delta/8)*Pixels->Width,
				//(Coordinates->X + 7*Delta/8)+(Coordinates->Y + 3*Delta/8)*Pixels->Width,
				//(Coordinates->X + Delta/8)+(Coordinates->Y + 5*Delta/8)*Pixels->Width,
				//(Coordinates->X + 3*Delta/8)+(Coordinates->Y + 5*Delta/8)*Pixels->Width,
				//(Coordinates->X + 5*Delta/8)+(Coordinates->Y + 5*Delta/8)*Pixels->Width,
				//(Coordinates->X + 7*Delta/8)+(Coordinates->Y + 5*Delta/8)*Pixels->Width,
				//(Coordinates->X + Delta/8)+(Coordinates->Y + 7*Delta/8)*Pixels->Width,
				//(Coordinates->X + 3*Delta/8)+(Coordinates->Y + 7*Delta/8)*Pixels->Width,
				//(Coordinates->X + 5*Delta/8)+(Coordinates->Y + 7*Delta/8)*Pixels->Width,
				//(Coordinates->X + 7*Delta/8)+(Coordinates->Y + 7*Delta/8)*Pixels->Width,
				(Coordinates->X + (Delta>>3))+(Coordinates->Y + (Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>2)+(Delta>>3))+(Coordinates->Y + (Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>1)+(Delta>>3))+(Coordinates->Y + (Delta>>3))*Pixels->Width,
				(Coordinates->X + Delta-(Delta>>3))+(Coordinates->Y + (Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>3))+(Coordinates->Y + (Delta>>2)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>2)+(Delta>>3))+(Coordinates->Y + (Delta>>2)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>1)+(Delta>>3))+(Coordinates->Y + (Delta>>2)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + Delta-(Delta>>3))+(Coordinates->Y + (Delta>>2)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>3))+(Coordinates->Y + (Delta>>1)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>2)+(Delta>>3))+(Coordinates->Y + (Delta>>1)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>1)+(Delta>>3))+(Coordinates->Y + (Delta>>1)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + Delta-(Delta>>3))+(Coordinates->Y + (Delta>>1)+(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>3))+(Coordinates->Y + Delta-(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>2)+(Delta>>3))+(Coordinates->Y + Delta-(Delta>>3))*Pixels->Width,
				(Coordinates->X + (Delta>>1)+(Delta>>3))+(Coordinates->Y + Delta-(Delta>>3))*Pixels->Width,
				(Coordinates->X + Delta-(Delta>>3))+(Coordinates->Y + Delta-(Delta>>3))*Pixels->Width,
			};
			for(int* index = indexes; index < indexes+16; index++){
				Score += CheckColour(Pixels->Pixmap[*index], *index, Range);
			}
		}
		else if(Mode == 1){
			int x = Coordinates->X + Delta/2;
			for(int y = Coordinates->Y; y < Coordinates->Y + Delta; y++){
				Score += CheckColour(Pixels->Pixmap[x + y*Pixels->Width], x + y*Pixels->Width, Range);
			}
			int y = Coordinates->Y + Delta/2;
			for(int x = Coordinates->X; x < Coordinates->X + Delta; x++){
				Score += CheckColour(Pixels->Pixmap[x + y*Pixels->Width], x + y*Pixels->Width, Range);
			}
		}
		else{
			for(int x = Coordinates->X; x < Coordinates->X + Delta; x++){
				for(int y = Coordinates->Y; y < Coordinates->Y + Delta; y++){
					Score += CheckColour(Pixels->Pixmap[x + y*Pixels->Width], x + y*Pixels->Width, Range);
				}
			}
		}
		return Score;
	}

	void Converge(pixmap *Pixels, coordinate *PCoordinate, int SquareSize, colourRange *Range){
		/*
		 *	Name:				Converge
		 *
		 *	Description:		Makes a given tile converge to the highest score
		 *
		 *	Full Description:	This function tries out a displacement in 4 directions, takes the best one and starts a recursion
		 *
		 *	Inputs:
		 *		Pixels:			Pixmap containing the whole image
		 *		*PCoordinate:	Pointer to the tile to make converge
		 *		SquareSize:		Size of the tile
		 *		Range:			Colour range to calculate the score with
		 */
		coordinate TempCoords[4] = {
			{PCoordinate->X + 1, PCoordinate->Y, 0},
			{PCoordinate->X, PCoordinate->Y + 1, 0},
			{PCoordinate->X - 1, PCoordinate->Y, 0},
			{PCoordinate->X, PCoordinate->Y - 1, 0},
		};
		for(int level = 1; level >= 0; level--){
			for(int i = 4; i--; ){
				TempCoords[i].Score = GetScore(Pixels, &TempCoords[i], SquareSize, Range, level);
				if(TempCoords[i].Score > PCoordinate->Score){
					*PCoordinate = TempCoords[i];
					Converge(Pixels, PCoordinate, SquareSize, Range);
					return;
				}
			}
		}
	}

	void FindBall(pixmap *Pixels, coordinate *PBall, coordinateRange *Table, int BallDiameter, colourRange *Range){
		/*
		 *	Name:				FindBall
		 *
		 *	Description:		Finds a given ball in a given pixmap
		 *
		 *	Full Description:	Separates the pixmap in tiles of the size of the ball.
		 *						Makes interesting tiles converge to the best score.
		 *
		 *	Inputs:
		 *		Pixels:			Image data
		 *		PBall:			Pointer t the coordinates of the ball we want to find
		 *		Table:			Area on which to search
		 *		BallDiameter:	Size of the ball
		 *		Range:			Colour range of the ball
		 */
		coordinate TileAmount;
		TileAmount.X = (Table->Max.X-Table->Min.X) / BallDiameter + ((Table->Max.X-Table->Min.X) % BallDiameter == 0 ? 0: 1);
		TileAmount.Y = (Table->Max.Y-Table->Min.Y) / BallDiameter + ((Table->Max.Y-Table->Min.Y) % BallDiameter == 0 ? 0: 1);
		for(int TileY = TileAmount.Y; TileY--; ){
			int y = Table->Min.Y + TileY*BallDiameter;
			if(y+BallDiameter > Table->Max.Y) y -= y + BallDiameter - Table->Max.Y;
			for(int TileX = TileAmount.X; TileX--; ){
				int x = Table->Min.X + TileX*BallDiameter;
				if(x+BallDiameter > Table->Max.X) x -= x + BallDiameter - Table->Max.X;
				coordinate Tile = {x, y, 0};
				if(GetScore(Pixels, &Tile, BallDiameter, Range, 2)){
					Tile.Score = GetScore(Pixels, &Tile, BallDiameter, Range, 1);
					Converge(Pixels, &Tile, BallDiameter, Range);
					if(Tile.Score > PBall->Score){
						PBall->X = Tile.X;
						PBall->Y = Tile.Y;
						PBall->Score = Tile.Score;
					}
				}
			}
		}
	}

