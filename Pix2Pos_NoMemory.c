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
 *	Version:			3.2.2
 *
 *	Revision:			16.05.2020, ED, PrettyPrint and final revision
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MIN_BALL_DIAMETER 5
#define MAX_BALL_DIAMETER 20
#define MIN_IMAGE_WIDTH 10
#define MIN_IMAGE_HEIGHT 10
#define MAX_IMAGE_WIDTH 1000
#define MAX_IMAGE_HEIGHT 1000



/*Structure declaration*/
	typedef struct colour_t {int R, G, B;} colour_t;

	typedef struct colourRange_t {colour_t Min, Max;} colourRange_t;

	typedef struct coordinate_t {int X, Y, Score;} coordinate_t;

	typedef struct coordinateRange_t {coordinate_t Min, Max;} coordinateRange_t;

	typedef struct ball_t {colourRange_t *Range; coordinate_t Coordinates;} ball_t;

	typedef struct pixmap_t {unsigned int Width, Height, *Pixmap;} pixmap_t;



/*Function declaration*/
	/*Utils*/
		int readFile(unsigned int *ptr, int size, int amount, _Bool addition, FILE *File);

		colour_t int2Colour(int colourInt);

		int checkColour(int pixel, int index, colourRange_t *Range);

		coordinateRange_t buildNeighbourhood(coordinate_t *Center, coordinateRange_t *Limits, int size, int offset);

		int getScore(pixmap_t *Pixels, coordinate_t *Coordinates, int delta, colourRange_t *Range, int mode);


	int readCommandLine(int argc, char **argv, coordinateRange_t *Table, colourRange_t *RBall, colourRange_t *YBall, colourRange_t *WBall, colourRange_t *BG, int *ballDiameter);

	int readPixmap(pixmap_t *Pixels, coordinateRange_t *Table);

	int findAllBalls(pixmap_t *Pixels, coordinateRange_t *Table, ball_t *Red, ball_t *Yellow, ball_t *White, int ballDiameter);

	void findBall(pixmap_t *Pixels, ball_t *PBall, coordinateRange_t *Table, int ballDiameter);

	void converge(pixmap_t *Pixels, coordinate_t *PCoordinate, int squareSize, colourRange_t *Range, coordinateRange_t *Limit);

	int writePosTxt(ball_t *Red, ball_t *Yellow, ball_t *White);




/*Main*/
int main(int argc, char **argv){
	/*Variable Declaration*/
		coordinateRange_t Table;
		colourRange_t RBall;
		colourRange_t YBall;
		colourRange_t WBall;
		colourRange_t BG;
		int ballDiameter;
		pixmap_t Pixels;
		ball_t Red = {&RBall, {-1, -1, -1}};
		ball_t Yellow = {&YBall, {-1, -1, -1}};
		ball_t White = {&WBall, {-1, -1, -1}};
	/*Read arguments*/
		if(readCommandLine(argc, argv, &Table, &RBall, &YBall, &WBall, &BG, &ballDiameter)) return -1;
	/*Read Pixmap.bin*/
		if(readPixmap(&Pixels, &Table)) return -1;
	/*Find the balls*/
		if(findAllBalls(&Pixels, &Table, &Red, &Yellow, &White, ballDiameter)) return -1;
	/*Open and write in Pos.txt*/
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
		if(RBall->Min.R < 0 || RBall->Min.G < 0 || RBall->Min.B < 0 || RBall->Max.R > 255 || RBall->Max.G > 255 || RBall->Max.B > 255 || RBall->Min.R > RBall->Max.R || RBall->Min.G > RBall->Max.G || RBall->Min.B > RBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as red ball colour range, cannot continue\n");
			return -1;
		}
		if(YBall->Min.R < 0 || YBall->Min.G < 0 || YBall->Min.B < 0 || YBall->Max.R > 255 || YBall->Max.G > 255 || YBall->Max.B > 255 || YBall->Min.R > YBall->Max.R || YBall->Min.G > YBall->Max.G || YBall->Min.B > YBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as yellow ball colour range, cannot continue\n");
			return -1;
		}
		if(WBall->Min.R < 0 || WBall->Min.G < 0 || WBall->Min.B < 0 || WBall->Max.R > 255 || WBall->Max.G > 255 || WBall->Max.B > 255 || WBall->Min.R > WBall->Max.R || WBall->Min.G > WBall->Max.G || WBall->Min.B > WBall->Max.B){
			fprintf(stderr, "Error : invalid values passed as white ball colour range, cannot continue\n");
			return -1;
		}
		if(BG->Min.R < 0 || BG->Min.G < 0 || BG->Min.B < 0 || BG->Max.R > 255 || BG->Max.G > 255 || BG->Max.B > 255 || BG->Min.R > BG->Max.R || BG->Min.G > BG->Max.G || BG->Min.B > BG->Max.B){
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
	 *	Description:		Reads the pixmap and checks it's validity
	 *
	 *	Inputs:
	 *		Pixels:			Pixmap data
	 *		Table:			Table coordinates
	 *	Return:			
	 *		return:			0 if all worked perfectly, else -1
	 *	Error:
	 *						Couldn't open Pixmap.bin
	 *						Couldn't close Pixmap.bin
	 *						Invalid image size
	 */
	int readPixmap(pixmap_t *Pixels, coordinateRange_t *Table){
		/*Open Pixmap.bin and verify width and heigth*/
			FILE *PixmapBin;
			PixmapBin = fopen("Pixmap.bin", "rb");
			if(PixmapBin == NULL){
				perror("Error : couldn't open Pixmap.bin");
				return -1;
			}
			if(readFile(&(Pixels->Width), sizeof(unsigned int), 1, 0, PixmapBin)) {if(fclose(PixmapBin)) perror("Error : couldn't close Pixmap.bin"); return -1;}
			if(readFile(&(Pixels->Height), sizeof(unsigned int), 1, 0, PixmapBin)) {if(fclose(PixmapBin)) perror("Error : couldn't close Pixmap.bin"); return -1;}
			if(Pixels->Width < MIN_IMAGE_WIDTH || Pixels->Height < MIN_IMAGE_HEIGHT || Pixels->Width > MAX_IMAGE_WIDTH || Pixels->Height > MAX_IMAGE_HEIGHT || Pixels->Width < Table->Max.X || Pixels->Height < Table->Max.Y){
				if(fclose(PixmapBin)) perror("Error : couldn't close Pixmap.bin");
				fprintf(stderr, "Error : invalid values passed as image size, cannot continue\n");
				return -1;
			}
		/*Read the pixels and close the file*/
			Pixels->Pixmap = malloc(sizeof(unsigned int)*(Pixels->Width*Pixels->Height+1));
			if(readFile(Pixels->Pixmap, sizeof(unsigned int), Pixels->Width*Pixels->Height, 1, PixmapBin)){
				free(Pixels->Pixmap);
				if(fclose(PixmapBin)) perror("Error : couldn't close Pixmap.bin");
				return -1;
			}
			if(fclose(PixmapBin)){perror("Error : couldn't close Pixmap.bin"); return -1;}
			return 0;
	}

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
	 *		*ptr:			Pointer to store the red data in
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
	 *	Description:		Converts a colour integer to RGB
	 *
	 *	Inputs:
	 *		colourInt:		Colour integer
	 *	Output:			
	 *		ColourRGB:		Colour RGB
	 */
	colour_t int2Colour(int colourInt) {
		colour_t ColourRGB = {-1, -1, -1};
		/*Detecting if int not in correct range*/
			if(colourInt < 0 || colourInt > 16777215){
				return ColourRGB;
			}
		/*Calculating colour*/
			ColourRGB.R = (colourInt & 0x00FF0000) >> 16;
			ColourRGB.G = (colourInt & 0x0000FF00) >> 8;
			ColourRGB.B = (colourInt & 0x000000FF);
		return ColourRGB;
	}

	/*
	 *	Name:				checkColour
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
	int checkColour(int pixel, int index, colourRange_t *Range){
		colour_t PixelColour = int2Colour(pixel);
		if(PixelColour.R < 0 || PixelColour.G < 0 || PixelColour.B < 0) fprintf(stderr, "Error : colour error at pixel %d ignoring pixel\n", index);
		if(PixelColour.R >= Range->Min.R && PixelColour.R <= Range->Max.R 
			&& PixelColour.G >= Range->Min.G && PixelColour.G <= Range->Max.G 
			&& PixelColour.B >= Range->Min.B && PixelColour.B <= Range->Max.B) 
				return 1;
		else return 0;
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
					Score += checkColour(Pixels->Pixmap[*index], *index, Range);
				}
				break;
			case 1:;
				int x = Coordinates->X + delta/2;
				for(int y = Coordinates->Y; y < Coordinates->Y + delta; y++){
					Score += checkColour(Pixels->Pixmap[x + y*Pixels->Width], x + y*Pixels->Width, Range);
				}
				int y = Coordinates->Y + delta/2;
				for(int x = Coordinates->X; x < Coordinates->X + delta; x++){
					Score += checkColour(Pixels->Pixmap[x + y*Pixels->Width], x + y*Pixels->Width, Range);
				}
				break;
			default:
				for(int x = Coordinates->X; x < Coordinates->X + delta; x++){
					for(int y = Coordinates->Y; y < Coordinates->Y + delta; y++){
						Score += checkColour(Pixels->Pixmap[x + y*Pixels->Width], x + y*Pixels->Width, Range);
					}
				}
		}
		return Score;
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
		// First calculating how many tiles of the size of the ball to separate the table to.
		coordinate_t TileAmount; 
		TileAmount.X = ceil((Table->Max.X-Table->Min.X) / (double)ballDiameter);
		TileAmount.Y = ceil((Table->Max.Y-Table->Min.Y) / (double)ballDiameter);
		//Iterating through the tiles
		for(int TileY = 0; TileY < TileAmount.Y; TileY++){
			//Calculating the y coordinate of the tile.
			int y = Table->Min.Y + TileY*ballDiameter; 
			//Detect if the tile is too far down and rectify.
			if(y+ballDiameter > Table->Max.Y) y = Table->Max.Y - ballDiameter; 
			//Iterating through the tiles
			for(int TileX = 0; TileX < TileAmount.X; TileX++){ 
				//Calculating the x coordinate of the tile.
				int x = Table->Min.X + TileX*ballDiameter; 
				//Detect if the tile is too far right and rectify.
				if(x+ballDiameter > Table->Max.X) x = Table->Max.X - ballDiameter;
				//Create the tile
				coordinate_t Tile = {x, y, 0};
				//Detect if the tile is interesting by getScore mode 2
				if(getScore(Pixels, &Tile, ballDiameter, PBall->Range, 2)){ 
					Tile.Score = getScore(Pixels, &Tile, ballDiameter, PBall->Range, 1);
					//Make tile converge to the best Score
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
	 *	Name:				findAllBalls
	 *
	 *	Description:		Finds all the balls
	 *
	 *	Inputs:
	 *		Pixels:			Image data
	 *		Table:			Area on which to search
	 *		Red/Yellow/White:	Ball Data
	 *		ballDiameter:	Size of the ball
	 *	Errors:
	 *						Couldn't close Pos.txt
	 *						Ball missing
	 *						Balls overlapping
	 */
	int findAllBalls(pixmap_t *Pixels, coordinateRange_t *Table, ball_t *Red, ball_t *Yellow, ball_t *White, int ballDiameter){
		/*Try to find the balls on the whole table*/
			findBall(Pixels, Red, Table, ballDiameter);
			findBall(Pixels, Yellow, Table, ballDiameter);
			findBall(Pixels, White, Table, ballDiameter);
			free(Pixels->Pixmap);
		/*Check if the balls are all here and if they are overlapping*/
			if(Red->Coordinates.Score < 0){
				fprintf(stderr, "Error : red ball missing\n");
				return -1;
			}
			if(Yellow->Coordinates.Score < 0){
				fprintf(stderr, "Error : yellow ball missing\n");
				return -1;
			}
			if(White->Coordinates.Score < 0){
				fprintf(stderr, "Error : white ball missing\n");
				return -1;
			}
			if(abs(Red->Coordinates.X - White->Coordinates.X) < ballDiameter && abs(Red->Coordinates.Y - White->Coordinates.Y) < ballDiameter){
				fprintf(stderr, "Error : white ball and red ball overlapping. Continuing\n");
			}
			if(abs(Red->Coordinates.X - Yellow->Coordinates.X) < ballDiameter && abs(Red->Coordinates.Y - Yellow->Coordinates.Y) < ballDiameter){
				fprintf(stderr, "Error : yellow ball and red ball overlapping. Continuing\n");
			}
			if(abs(Yellow->Coordinates.X - White->Coordinates.X) < ballDiameter && abs(Yellow->Coordinates.Y - White->Coordinates.Y) < ballDiameter){
				fprintf(stderr, "Error : white ball and yellow ball overlapping. Continuing\n");
			}
			return 0;
	}

	/*
	 *	Name:				writePosTxt
	 *
	 *	Description:		Writes the ball data in Pos.txt
	 *
	 *	Inputs:
	 *		Red/Yellow/White 	Ball data
	 *	Errors:
	 *						Couldn't open Pos.txt
	 *						Couldn't close Pos.txt
	 *						Couldn't write in Pos.txt
	 */
	int writePosTxt(ball_t *Red, ball_t *Yellow, ball_t *White){
		FILE *PosTxt = fopen("Pos.txt", "w");
		if(PosTxt == NULL){
			perror("Error : couldn't open Pos.txt");
			return -1;
		}
		if(1 > fprintf(PosTxt, "Red: %d, %d, %d\nYellow: %d, %d, %d\nWhite: %d, %d, %d", Red->Coordinates.X, Red->Coordinates.Y, Red->Coordinates.Score, Yellow->Coordinates.X, Yellow->Coordinates.Y, Yellow->Coordinates.Score, White->Coordinates.X, White->Coordinates.Y, White->Coordinates.Score)){
			fprintf(stderr, "Error : couldn't write in Pos.txt\n");
			int err = ferror(PosTxt);
			if(err){
				fprintf(stderr, "Cause: error %d\n", err);
				clearerr(PosTxt);
			}
			return -1;
		}
		if(fclose(PosTxt)){perror("Error: couldn't close Pos.txt"); return -1;}
		return 0;
	}