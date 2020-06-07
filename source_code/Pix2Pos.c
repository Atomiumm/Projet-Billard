/*
 *	File:				Pix2Pos.c
 *
 *	Description: 		Billiard configuration analysis: Balls finder
 *	
 *	Full Description: 	Reads a pixmap.bin file. 
 *						Reads the image data. 
 *						Separates the image in tiles.
 *						Makes the interesting tiles converge to the balls. 
 *						Stores the ball coordinates in pos.txt
 *
 *	Dependancy:			pixmap.bin
 *
 *	Authors:			Dufour Edouard; Rochet Corentin		No copyright
 *
 *	Version:			3.3.0
 *
 *	Revision:			06.06.2020, ED, PrettyPrint and final revision
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIN_BALL_DIAMETER 5
#define MAX_BALL_DIAMETER 20
#define MIN_IMAGE_WIDTH 10
#define MIN_IMAGE_HEIGHT 10
#define MAX_IMAGE_WIDTH 1000
#define MAX_IMAGE_HEIGHT 1000


/*Structure declaration*/
	typedef struct {unsigned char R, G, B;} colour_t;

	typedef struct {colour_t Min, Max;} colourRange_t;

	typedef struct {int X, Y, Score;} coordinate_t;

	typedef struct {coordinate_t Min, Max;} coordinateRange_t;

	typedef struct {colourRange_t *Range; coordinate_t Coordinates;} ball_t;

	typedef struct {unsigned int Width, Height; colour_t *Pixmap;} pixmap_t;

/*Function declaration*/
	int readCommandLine(int argc, char **argv, coordinateRange_t *Table, colourRange_t *RBall, colourRange_t *YBall, colourRange_t *WBall, colourRange_t *BG, int *ballDiameter);
	int readPixmap(pixmap_t *Pixels, coordinateRange_t *Table);
		int readImageSize(pixmap_t *Pixels, coordinateRange_t *Table, FILE *PixmapBin);
		int readPixels(pixmap_t *Pixels, FILE *PixmapBin);
		int readFile(unsigned int *ptr, int size, int amount, _Bool addition, FILE *File);
		void int2Colour(unsigned int *colourInt, pixmap_t *Pixels);
	int findAllBalls(pixmap_t *Pixels, coordinateRange_t *Table, ball_t *Red, ball_t *Yellow, ball_t *White, int ballDiameter);
		void findBall(pixmap_t *Pixels, ball_t *PBall, coordinateRange_t *Table, int ballDiameter);
		void converge(pixmap_t *Pixels, coordinate_t *PCoordinate, int squareSize, colourRange_t *Range, coordinateRange_t *Limit);
		int getScore(pixmap_t *Pixels, coordinate_t *Coordinates, int delta, colourRange_t *Range, int mode);
		int checkColour(colour_t PixelColour, colourRange_t *Range);
	int writePosTxt(ball_t *Red, ball_t *Yellow, ball_t *White);




int main(int argc, char **argv){
	/*Variable Declaration*/
		coordinateRange_t Table;
		colourRange_t RBall, YBall, WBall, BG;
		int ballDiameter;
		pixmap_t Pixels;
		ball_t Red = {&RBall, {-1, -1, -1}};
		ball_t Yellow = {&YBall, {-1, -1, -1}};
		ball_t White = {&WBall, {-1, -1, -1}};
	/*Read arguments*/
		if(readCommandLine(argc, argv, &Table, &RBall, &YBall, &WBall, &BG, &ballDiameter)) return -1;
	/*Read pixmap.bin*/
		if(readPixmap(&Pixels, &Table)) return -1;
	/*Find the balls*/
		if(findAllBalls(&Pixels, &Table, &Red, &Yellow, &White, ballDiameter)) return -1;
	/*Open and write in pos.txt*/
		if(writePosTxt(&Red, &Yellow, &White)) return -1;
	return 0;
}



/*Function initialization*/
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
	 *		ballDiameter:	Size of the ball
	 *	Return:			
	 *		return:			0 if all worked perfectly, else -1
	 *	Errors:
	 *		Command line:	Not the right amount of arguments
	 *		Table:			Table size < 0 or min > max
	 *		Balls:			Colour range > 255 or min > max
	 *		Background:		Colour range > 255 or min > max
	 *		Balldiameter:	Diameter out of bounds or bigger than table
	 */
	int readCommandLine(int argc, char **argv, coordinateRange_t *Table, colourRange_t *RBall, colourRange_t *YBall, colourRange_t *WBall, colourRange_t *BG, int *ballDiameter){
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
			*ballDiameter = atoi(argv[29]);
		}
		else {
			fprintf(stderr, "Error : invalid number of argument, cannot continue\n");
			return -1;
		}
		if(Table->Min.X < 0 || Table->Min.Y < 0 || Table->Min.X > Table->Max.X || Table->Min.Y > Table->Max.Y){
			fprintf(stderr, "Error : invalid values passed as table size, cannot continue\n");
			return -1;
		}
		if(RBall->Max.R > 255 || RBall->Max.G > 255 || RBall->Max.B > 255 || RBall->Min.R > RBall->Max.R || RBall->Min.G > RBall->Max.G || RBall->Min.B > RBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as red ball colour range, cannot continue\n");
			return -1;
		}
		if(YBall->Max.R > 255 || YBall->Max.G > 255 || YBall->Max.B > 255 || YBall->Min.R > YBall->Max.R || YBall->Min.G > YBall->Max.G || YBall->Min.B > YBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as yellow ball colour range, cannot continue\n");
			return -1;
		}
		if(WBall->Max.R > 255 || WBall->Max.G > 255 || WBall->Max.B > 255 || WBall->Min.R > WBall->Max.R || WBall->Min.G > WBall->Max.G || WBall->Min.B > WBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as white ball colour range, cannot continue\n");
			return -1;
		}
		if(BG->Max.R > 255 || BG->Max.G > 255 || BG->Max.B > 255 || BG->Min.R > BG->Max.R || BG->Min.G > BG->Max.G || BG->Min.B > BG->Max.B){
			fprintf(stderr, "Error : invalid values passed as background colour range, cannot continue\n");
			return -1;
		}
		if(Table->Max.X - Table->Min.X < *ballDiameter || Table->Max.Y - Table->Min.Y < *ballDiameter){
			fprintf(stderr, "Error : invalid values passed as table and ball size, ball is bigger than table, cannot continue\n");
			return -1;
		}
		if(*ballDiameter < MIN_BALL_DIAMETER || *ballDiameter > MAX_BALL_DIAMETER){
			fprintf(stderr, "Error : invalid values passed as ball size, cannot continue\n");
			return -1;
		}
		return 0;
	}

	/*
	 *	Name:				readPixmap
	 *
	 *	Description:		Reads the image size, then the pixels and checks the images validity
	 *
	 *	Inputs:
	 *		Pixels:			Pixmap data
	 *		Table:			Table coordinates
	 *	Return:			
	 *		return:			0 if all worked perfectly, else -1
	 *	Error:
	 *						Couldn't open pixmap.bin
	 *						Couldn't close pixmap.bin
	 */
	int readPixmap(pixmap_t *Pixels, coordinateRange_t *Table){
		FILE *PixmapBin;
		PixmapBin = fopen("pixmap.bin", "rb");
		if(PixmapBin == NULL){
			perror("Error : couldn't open pixmap.bin");
			return -1;
		}
		if(readImageSize(Pixels, Table, PixmapBin)) goto problem;
		if(readPixels(Pixels, PixmapBin)) goto problem;
		if(fclose(PixmapBin)) perror("Error : couldn't close pixmap.bin");
		return 0;

		problem : {if(fclose(PixmapBin)) perror("Error : couldn't close pixmap.bin"); return -1;}
	}

	/*
	 *	Name:				readImageSize
	 *
	 *	Description:		Reads the image size and checks it's validity
	 *
	 *	Inputs:
	 *		Pixels:			Pixmap data
	 *		Table:			Table coordinates
	 *		PixmapBin:		File to read from
	 *	Return:			
	 *		return:			0 if all worked perfectly, else -1
	 *	Error:
	 *						Invalid image size
	 */
	int readImageSize(pixmap_t *Pixels, coordinateRange_t *Table, FILE *PixmapBin){
		if(readFile(&(Pixels->Width), sizeof(unsigned int), 1, 0, PixmapBin)) return -1;
		if(readFile(&(Pixels->Height), sizeof(unsigned int), 1, 0, PixmapBin)) return -1;
		if(Pixels->Width < MIN_IMAGE_WIDTH || Pixels->Height < MIN_IMAGE_HEIGHT || Pixels->Width > MAX_IMAGE_WIDTH || Pixels->Height > MAX_IMAGE_HEIGHT || Pixels->Width < Table->Max.X || Pixels->Height < Table->Max.Y){
			fprintf(stderr, "Error : invalid values passed as image size, cannot continue\n");
			return -1;
		}
		return 0;
	}

	/*
	 *	Name:				readPixels
	 *
	 *	Description:		Reads the image pixels as ints then converts them to colour_t
	 *
	 *	Inputs:
	 *		Pixels:			Pixmap data
	 *		PixmapBin:		File to read from
	 *	Return:			
	 *		return:			0 if all worked perfectly, else -1
	 *	Error:
	 *						Array allocation problem
	 */
	int readPixels(pixmap_t *Pixels, FILE *PixmapBin){
		unsigned int *pixmap = malloc(sizeof(unsigned int)*(Pixels->Width*Pixels->Height+1));
		Pixels->Pixmap = malloc(sizeof(colour_t)*(Pixels->Width*Pixels->Height));
		if(pixmap == NULL || Pixels->Pixmap == NULL) {fprintf(stderr, "Error : Allocation problem.\n"); goto problem;}
		if(readFile(pixmap, sizeof(unsigned int), Pixels->Width*Pixels->Height, 1, PixmapBin)) goto problem;
		int2Colour(pixmap, Pixels);
		free(pixmap);
		return 0;

		problem: {free(pixmap); free(Pixels->Pixmap); return -1;}
	}

	/*
	 *	Name:				readFile
	 *
	 *	Description:		Reads a certain amount of a given file and handles errors
	 *
	 *	Full Description:	Reads a given amount of elements of given size in a given file and stores the result in a pointer.
	 *						Makes whole error gestion
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
	int readFile(unsigned int *ptr, int size, int amount, _Bool addition, FILE *File){
		int temp = fread(ptr, size, amount+addition, File);
		if(temp != amount){
			fprintf(stderr, "Read %d elements, expected %d\n", temp, amount);
			if(feof(File)) fprintf(stderr, "Cause : end of file reached\n");
			int err = ferror(File);
			if(err){
				fprintf(stderr, "Cause : error %d\n", err);
				clearerr(File);
			}
			return -1;
		}
		return 0;
	}

	/*
	 *	Name:				int2Colour
	 *
	 *	Description:		converts a given unsigned int array to colour_t array
	 *
	 *	Inputs:
	 *		colourInt:		unsigned int input array
	 *		Pixels:			pixmap_t structure to store the resulting array into
	 *	Errors:
	 *						Invalid pixel
	 */
	void int2Colour(unsigned int *colourInt, pixmap_t *Pixels){
		for(int i = 0; i < Pixels->Width * Pixels->Height; i++){
			if(colourInt[i] < 0 || colourInt[i] > 16777215){
				fprintf(stderr, "Error: Pixel error at index %d setting to black\n", i);
				colourInt[i] = 0;
			}
			else{
				Pixels->Pixmap[i].R = (colourInt[i] & 0x00FF0000) >> 16;
				Pixels->Pixmap[i].G = (colourInt[i] & 0x0000FF00) >> 8;
				Pixels->Pixmap[i].B = (colourInt[i] & 0x000000FF);
			}
		}
	}

	/*
	 *	Name:				findAllBalls
	 *
	 *	Description:		Finds all the balls
	 *
	 *	Full Description:	Looks for the balls on the table.
	 *						Checks if the balls are found and if they are overlapping.
	 *
	 *	Inputs:
	 *		Pixels:			Image data
	 *		Table:			Area on which to search
	 *		Red/Yellow/White:	Ball Data
	 *		ballDiameter:	Size of the ball
	 *	Errors:
	 *						Ball missing
	 *						Balls overlapping
	 */
	int findAllBalls(pixmap_t *Pixels, coordinateRange_t *Table, ball_t *Red, ball_t *Yellow, ball_t *White, int ballDiameter){
		findBall(Pixels, Red, Table, ballDiameter);
		findBall(Pixels, Yellow, Table, ballDiameter);
		findBall(Pixels, White, Table, ballDiameter);
		free(Pixels->Pixmap);
		if(Red->Coordinates.Score < 0) {fprintf(stderr, "Error : red ball missing\n"); return -1;}
		if(Yellow->Coordinates.Score < 0) {fprintf(stderr, "Error : yellow ball missing\n"); return -1;}
		if(White->Coordinates.Score < 0) {fprintf(stderr, "Error : white ball missing\n"); return -1;}
		if(abs(Red->Coordinates.X - White->Coordinates.X) < ballDiameter && abs(Red->Coordinates.Y - White->Coordinates.Y) < ballDiameter)
			fprintf(stderr, "Error : white ball and red ball overlapping. Continuing\n");
		if(abs(Red->Coordinates.X - Yellow->Coordinates.X) < ballDiameter && abs(Red->Coordinates.Y - Yellow->Coordinates.Y) < ballDiameter)
			fprintf(stderr, "Error : yellow ball and red ball overlapping. Continuing\n");
		if(abs(Yellow->Coordinates.X - White->Coordinates.X) < ballDiameter && abs(Yellow->Coordinates.Y - White->Coordinates.Y) < ballDiameter)
			fprintf(stderr, "Error : white ball and yellow ball overlapping. Continuing\n");
		return 0;
	}

	/*
	 *	Name:				findBall
	 *
	 *	Description:		Finds a given ball in a given pixmap
	 *
	 *	Full Description:	Separates the pixmap in tiles of the size of the ball.
	 *						Makes interesting tiles converge to the best score.
	 *
	 *	Inputs:
	 *		Pixels:			Image data
	 *		PBall:			Ball Data
	 *		Table:			Area on which to search
	 *		ballDiameter:	Size of the ball
	 */
	void findBall(pixmap_t *Pixels, ball_t *PBall, coordinateRange_t *Table, int ballDiameter){
		//used to calculate x and y coordinates from tile number
		//limits the coordinates to inside the table
		int getCoordinate(int min, int max, int size, int position){
			int x = min + position*size;
			if(x+size > max) x = max - size;
			return x;
		}
		//calculates the number of tiles (x and y directions)
		coordinate_t TileAmount = {
			ceil((Table->Max.X-Table->Min.X) / (double)ballDiameter),
			ceil((Table->Max.Y-Table->Min.Y) / (double)ballDiameter)
		};
		for(int TileY = 0; TileY < TileAmount.Y; TileY++){
			int y = getCoordinate(Table->Min.Y, Table->Max.Y, ballDiameter, TileY);
			for(int TileX = 0; TileX < TileAmount.X; TileX++){
				int x = getCoordinate(Table->Min.X, Table->Max.X, ballDiameter, TileX);
				coordinate_t Tile = {x, y, 0};
				//Detect if the tile is interesting by getScore mode 2
				if(getScore(Pixels, &Tile, ballDiameter, PBall->Range, 2)){ 
					Tile.Score = getScore(Pixels, &Tile, ballDiameter, PBall->Range, 1);
					converge(Pixels, &Tile, ballDiameter, PBall->Range, Table); 
					if(Tile.Score > PBall->Coordinates.Score){
						PBall->Coordinates.X = Tile.X;
						PBall->Coordinates.Y = Tile.Y;
						PBall->Coordinates.Score = Tile.Score;
					}
				}
			}
		}
	}

	/*
	 *	Name:				converge
	 *
	 *	Description:		Makes a given tile converge to the highest score
	 *
	 *	Full Description:	This function tries out a displacement in 4 directions, takes the best one and starts a recursion
	 *
	 *	Inputs:
	 *		Pixels:			Pixmap containing the whole image
	 *		*PCoordinate:	Pointer to the tile to make converge
	 *		squareSize:		Size of the tile
	 *		Range:			Colour range to calculate the score with
	 *		Limit:			Limits to prevent convergence outside of them
	 */
	void converge(pixmap_t *Pixels, coordinate_t *PCoordinate, int squareSize, colourRange_t *Range, coordinateRange_t *Limit){
		coordinate_t TempCoords[4] = {
			{PCoordinate->X + 1 > Limit->Max.X ? Limit->Max.X : PCoordinate->X + 1, PCoordinate->Y, 0},
			{PCoordinate->X, PCoordinate->Y + 1 > Limit->Max.Y ? Limit->Max.Y : PCoordinate->Y + 1, 0},
			{PCoordinate->X - 1 < Limit->Min.X ? Limit->Min.X : PCoordinate->X - 1, PCoordinate->Y, 0},
			{PCoordinate->X, PCoordinate->Y - 1 < Limit->Min.Y ? Limit->Min.Y : PCoordinate->Y - 1, 0},
		};
		for(int mode = 1; mode >= 0; mode--){ //First converge by taking the score mode 1, then by taking the score mode 0
			for(int i = 4; i--; ){
				TempCoords[i].Score = getScore(Pixels, &TempCoords[i], squareSize, Range, mode);
				if(TempCoords[i].Score > PCoordinate->Score){
					*PCoordinate = TempCoords[i];
					converge(Pixels, PCoordinate, squareSize, Range, Limit);
					return;
				}
			}
		}
	}

	/*
	 *	Name:				getScore
	 *
	 *	Description:		Gets the score of a given square
	 *
	 *	Full Description:	This function calculates the score of an amount of pixel given by the mode
	 *						The higher the mode, the faster, but the less precise.
	 *		mode 0				All the pixels of the square
	 *		mode 1				Central cross of the same size as the square
	 *		mode 2				16 pixels uniformly distributed in the square
	 *
	 *	Inputs:
	 *		Pixels:			Pixmap
	 *		Coordinates:	Coordinates of the square to calculate the score of
	 *		delta:			Size of the square
	 *		Range:			ColourRange to calculate the score with
	 *		mode:			mode desscribed in the full description
	 *	Output:
	 *		Score:			Score
	 */
	int getScore(pixmap_t *Pixels, coordinate_t *Coordinates, int delta, colourRange_t *Range, int mode){
		int Score = 0;
		switch(mode){
			case 2:;
				int indexes[16] = {
					(Coordinates->X + delta/8)+(Coordinates->Y + delta/8)*Pixels->Width,
					(Coordinates->X + 3*delta/8)+(Coordinates->Y + delta/8)*Pixels->Width,
					(Coordinates->X + 5*delta/8)+(Coordinates->Y + delta/8)*Pixels->Width,
					(Coordinates->X + 7*delta/8)+(Coordinates->Y + delta/8)*Pixels->Width,
					(Coordinates->X + delta/8)+(Coordinates->Y + 3*delta/8)*Pixels->Width,
					(Coordinates->X + 3*delta/8)+(Coordinates->Y + 3*delta/8)*Pixels->Width,
					(Coordinates->X + 5*delta/8)+(Coordinates->Y + 3*delta/8)*Pixels->Width,
					(Coordinates->X + 7*delta/8)+(Coordinates->Y + 3*delta/8)*Pixels->Width,
					(Coordinates->X + delta/8)+(Coordinates->Y + 5*delta/8)*Pixels->Width,
					(Coordinates->X + 3*delta/8)+(Coordinates->Y + 5*delta/8)*Pixels->Width,
					(Coordinates->X + 5*delta/8)+(Coordinates->Y + 5*delta/8)*Pixels->Width,
					(Coordinates->X + 7*delta/8)+(Coordinates->Y + 5*delta/8)*Pixels->Width,
					(Coordinates->X + delta/8)+(Coordinates->Y + 7*delta/8)*Pixels->Width,
					(Coordinates->X + 3*delta/8)+(Coordinates->Y + 7*delta/8)*Pixels->Width,
					(Coordinates->X + 5*delta/8)+(Coordinates->Y + 7*delta/8)*Pixels->Width,
					(Coordinates->X + 7*delta/8)+(Coordinates->Y + 7*delta/8)*Pixels->Width,
				};
				for(int* index = indexes; index < indexes+16; index++){
					Score += checkColour(Pixels->Pixmap[*index], Range);
				}
				break;
			case 1:;
				int x = Coordinates->X + delta/2;
				for(int y = Coordinates->Y; y < Coordinates->Y + delta; y++){
					Score += checkColour(Pixels->Pixmap[x + y*Pixels->Width], Range);
				}
				int y = Coordinates->Y + delta/2;
				for(int x = Coordinates->X; x < Coordinates->X + delta; x++){
					Score += checkColour(Pixels->Pixmap[x + y*Pixels->Width], Range);
				}
				break;
			default:
				for(int x = Coordinates->X; x < Coordinates->X + delta; x++){
					for(int y = Coordinates->Y; y < Coordinates->Y + delta; y++){
						Score += checkColour(Pixels->Pixmap[x + y*Pixels->Width], Range);
					}
				}
		}
		return Score;
	}

	/*
	 *	Name:				checkColour
	 *
	 *	Description:		Checks if a given pixel is inside a given colour range
	 *
	 *	Inputs:
	 *		PixelColour:			Pixel to check
	 *		Range:			Colour range the pixel should be in
	 *	Return:			
	 *		return:			1 if the pixel is in the range, 0 else
	 */
	int checkColour(colour_t PixelColour, colourRange_t *Range){
		return (PixelColour.R >= Range->Min.R && PixelColour.R <= Range->Max.R 
			&& PixelColour.G >= Range->Min.G && PixelColour.G <= Range->Max.G 
			&& PixelColour.B >= Range->Min.B && PixelColour.B <= Range->Max.B);
	}

	/*
	 *	Name:				writePosTxt
	 *
	 *	Description:		Writes the ball data in pos.txt
	 *
	 *	Inputs:
	 *		Red/Yellow/White 	Ball data
	 *	Errors:
	 *						Couldn't open pos.txt
	 *						Couldn't close pos.txt
	 *						Couldn't write in pos.txt
	 */
	int writePosTxt(ball_t *Red, ball_t *Yellow, ball_t *White){
		FILE *PosTxt = fopen("pos.txt", "w");
		if(PosTxt == NULL) {perror("Error : couldn't open pos.txt"); return -1;}
		if(1 > fprintf(PosTxt, "Red: %d, %d, %d\nYellow: %d, %d, %d\nWhite: %d, %d, %d", Red->Coordinates.X, Red->Coordinates.Y, Red->Coordinates.Score, Yellow->Coordinates.X, Yellow->Coordinates.Y, Yellow->Coordinates.Score, White->Coordinates.X, White->Coordinates.Y, White->Coordinates.Score)){
			fprintf(stderr, "Error : couldn't write in pos.txt\n");
			int err = ferror(PosTxt);
			if(err){
				fprintf(stderr, "Cause: error %d\n", err);
				clearerr(PosTxt);
			}
			return -1;
		}
		if(fclose(PosTxt)) {perror("Error: couldn't close pos.txt"); return -1;}
		return 0;
	}