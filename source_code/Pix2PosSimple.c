#include <stdio.h>
#include <stdlib.h>


#define MIN_BALL_DIAMETER 5
#define MAX_BALL_DIAMETER 20
#define MIN_IMAGE_WIDTH 10
#define MIN_IMAGE_HEIGHT 10
#define MAX_IMAGE_WIDTH 1000
#define MAX_IMAGE_HEIGHT 1000

typedef struct {int R, G, B;} colour_t;
typedef struct {int X, Y, Score;} coordinate_t;
typedef struct {unsigned int Width, Height, *Pixmap;} pixmap_t;
typedef struct {coordinate_t TableMin, TableMax;
				colour_t RBallMin, RBallMax,
						YBallMin, YBallMax,
						WBallMin, WBallMax,
						BGMin, BGMax;
				int ballDiameter;} data_t;






int readArguments(int argc, char **argv, data_t *data){
	if(argc == 30){
		data->TableMin.Y   = atoi(argv[1]);
		data->TableMax.Y   = atoi(argv[2]);
		data->TableMin.X   = atoi(argv[3]);
		data->TableMax.X   = atoi(argv[4]);
		data->RBallMin.R   = atoi(argv[5]);
		data->RBallMax.R   = atoi(argv[6]);
		data->RBallMin.G   = atoi(argv[7]);
		data->RBallMax.G   = atoi(argv[8]);
		data->RBallMin.B   = atoi(argv[9]);
		data->RBallMax.B   = atoi(argv[10]);
		data->YBallMin.R   = atoi(argv[11]);
		data->YBallMax.R   = atoi(argv[12]);
		data->YBallMin.G   = atoi(argv[13]);
		data->YBallMax.G   = atoi(argv[14]);
		data->YBallMin.B   = atoi(argv[15]);
		data->YBallMax.B   = atoi(argv[16]);
		data->WBallMin.R   = atoi(argv[17]);
		data->WBallMax.R   = atoi(argv[18]);
		data->WBallMin.G   = atoi(argv[19]);
		data->WBallMax.G   = atoi(argv[20]);
		data->WBallMin.B   = atoi(argv[21]);
		data->WBallMax.B   = atoi(argv[22]);
		data->BGMin.R      = atoi(argv[23]);
		data->BGMax.R      = atoi(argv[24]);
		data->BGMin.G      = atoi(argv[25]);
		data->BGMax.G      = atoi(argv[26]);
		data->BGMin.B      = atoi(argv[27]);
		data->BGMax.B      = atoi(argv[28]);
		data->ballDiameter = atoi(argv[29]);
	}
	else {fprintf(stderr, "La ligne de commande me semble incomplète. Je ne peux pas continuer.\n");return -1;}

	if(data->ballDiameter < MIN_BALL_DIAMETER || data->ballDiameter > MAX_BALL_DIAMETER){fprintf(stderr, "Tu m'as donné une taille de boule bizarre espèce de malotru!\n");return -1;}

	return 0;
}

int readPixmap(pixmap_t *Pixels){
	FILE *PixmapBin;
	PixmapBin = fopen("Pixmap.bin", "rb");
	if(PixmapBin == NULL){perror("Je ne sais pas ce que tu fais avec le fichier Pixmap.bin mais je n'arrive pas a le trouver.");return -1;}
	if(fread(&(Pixels->Width), sizeof(unsigned int), 1, PixmapBin) != 1){
		fprintf(stderr, "Je n'arrive pas a trouver la largeur de l'image\n");
		if(feof(PixmapBin)) fprintf(stderr, "J'ai enfin atteint la fin de ce maudit fichier\n");
		int err = ferror(PixmapBin);
		if(err){
			fprintf(stderr, "Il y a une erreur bizarre: %d, cherche sur google\n", err);
			clearerr(PixmapBin);
		}
		goto problem;
	}
	if(fread(&(Pixels->Height), sizeof(unsigned int), 1, PixmapBin) != 1){
		fprintf(stderr, "Je n'arrive pas a trouver la hauteur de l'image\n");
		if(feof(PixmapBin)) fprintf(stderr, "J'ai enfin atteint la fin de ce maudit fichier\n");
		int err = ferror(PixmapBin);
		if(err){
			fprintf(stderr, "Il y a une erreur bizarre: %d, cherche sur google\n", err);
			clearerr(PixmapBin);
		}
		goto problem;
	}
	if(Pixels->Width < MIN_IMAGE_WIDTH || Pixels->Height < MIN_IMAGE_HEIGHT || Pixels->Width > MAX_IMAGE_WIDTH || Pixels->Height > MAX_IMAGE_HEIGHT){
		fprintf(stderr, "Tu m'as filé une image beaucoup trop grande sale con!\n");
		goto problem;
	}
	Pixels->Pixmap = malloc(sizeof(unsigned int)*(Pixels->Width*Pixels->Height+1));
	int temp = fread(Pixels->Pixmap, sizeof(unsigned int), Pixels->Width*Pixels->Height+1, PixmapBin);
	if(temp != Pixels->Width*Pixels->Height){
		fprintf(stderr, "J'ai lu %d pixels au lieu d'en lire %d. Bizarre n'est-ce pas?\n", temp, Pixels->Width*Pixels->Height);
		if(feof(PixmapBin)) fprintf(stderr, "J'ai enfin atteint la fin de ce maudit fichier\n");
		int err = ferror(PixmapBin);
		if(err){
			fprintf(stderr, "Il y a une erreur bizarre: %d, cherche sur google\n", err);
			clearerr(PixmapBin);
		}
		free(Pixels->Pixmap);
		goto problem;
	}
	if(fclose(PixmapBin)) perror("J'ai eu tellement de mal a ouvrir le fichier Pixmap.bin que je refuse de le fermer!");
	return 0;
	problem: {if(fclose(PixmapBin)) perror("J'ai eu tellement de mal a ouvrir le fichier Pixmap.bin que je refuse de le fermer!"); return -1;}
}

int checkColour(int pixel, colour_t *Min, colour_t *Max){
	colour_t PixelColour = {(pixel & 0x00FF0000) >> 16, (pixel & 0x0000FF00) >> 8, (pixel & 0x000000FF)};
	if(PixelColour.R >= Min->R && PixelColour.R <= Max->R && PixelColour.G >= Min->G && PixelColour.G <= Max->G && PixelColour.B >= Min->B && PixelColour.B <= Max->B) return 1;
	else return 0;
}

void checkBall(int x, int y, int ballDiameter, pixmap_t *Pixels, coordinate_t *Ball, colour_t *Min, colour_t *Max){
	int score = 0;
	for(int dx = x; dx < x + ballDiameter; dx++){
		for(int dy = y; dy < y + ballDiameter; dy++){
			if(checkColour(Pixels->Pixmap[dx+dy*Pixels->Width], Min, Max)){
				++score;
			}
		}
	}
	if(score > Ball->Score){
		Ball->X = x;
		Ball->Y = y;
		Ball->Score = score;
	}
}

int findBalls(pixmap_t *Pixels, data_t *data, coordinate_t *Red, coordinate_t *Yellow, coordinate_t *White){
	for(int x = data->TableMin.X; x < data->TableMax.X - data->ballDiameter; x++){
		for(int y = data->TableMin.Y; y < data->TableMax.Y - data->ballDiameter; y++){
			if(checkColour(Pixels->Pixmap[(x+data->ballDiameter/2)+(y+data->ballDiameter/2)*Pixels->Width], &(data->BGMin), &(data->BGMax))) continue;
			if(checkColour(Pixels->Pixmap[(x+data->ballDiameter/2)+(y+data->ballDiameter/2)*Pixels->Width], &(data->RBallMin), &(data->RBallMax))){
				checkBall(x, y, data->ballDiameter, Pixels, Red, &(data->RBallMin), &(data->RBallMax));
			}
			if(checkColour(Pixels->Pixmap[(x+data->ballDiameter/2)+(y+data->ballDiameter/2)*Pixels->Width], &(data->YBallMin), &(data->YBallMax))){
				checkBall(x, y, data->ballDiameter, Pixels, Yellow, &(data->YBallMin), &(data->YBallMax));
			}
			if(checkColour(Pixels->Pixmap[(x+data->ballDiameter/2)+(y+data->ballDiameter/2)*Pixels->Width], &(data->WBallMin), &(data->WBallMax))){
				checkBall(x, y, data->ballDiameter, Pixels, White, &(data->WBallMin), &(data->WBallMax));
			}
		}
	}
	free(Pixels->Pixmap);
	if(!Red->Score) {fprintf(stderr, "Il me manque une boule rouge\n"); return -1;}
	if(!Yellow->Score) {fprintf(stderr, "Il me manque une boule jaune\n"); return -1;}
	if(!White->Score) {fprintf(stderr, "Il me manque une boule blanche\n"); return -1;}
	if(abs(Red->X - White->X) < data->ballDiameter && abs(Red->Y - White->Y) < data->ballDiameter){
		fprintf(stderr, "Ma boule rouge et ma boule blanche se superposent. Cela ne va pas m'arreter!\n");
	}
	if(abs(Red->X - Yellow->X) < data->ballDiameter && abs(Red->Y - Yellow->Y) < data->ballDiameter){
		fprintf(stderr, "Ma boule rouge et ma boule jaune se superposent. Cela ne va pas m'arreter!\n");
	}
	if(abs(Yellow->X - White->X) < data->ballDiameter && abs(Yellow->Y - White->Y) < data->ballDiameter){
		fprintf(stderr, "Ma boule jaune et ma boule blanche se superposent. Cela ne va pas m'arreter!\n");
	}
	return 0;
}

int writePosTxt(coordinate_t *Red, coordinate_t *Yellow, coordinate_t *White){
	FILE *PosTxt = fopen("Pos.txt", "w");
	if(PosTxt == NULL) {perror("Je ne sais pas ce que tu fais avec le fichier Pos.txt mais je n'arrive pas a le trouver."); return -1;}
	if(!fprintf(PosTxt, "Red: %d, %d, %d\nYellow: %d, %d, %d\nWhite: %d, %d, %d", Red->X, Red->Y, Red->Score, Yellow->X, Yellow->Y, Yellow->Score, White->X, White->Y, White->Score)){
		fprintf(stderr, "Mon cher journal. Aujourd'hui j'ai essayé d'écrire dans Pos.txt mais je n'ai pas réussi. Je suis en profonde déprime.\n");
		int err = ferror(PosTxt);
		if(err){
			fprintf(stderr, "Il y a une erreur bizarre: %d, cherche sur google\n", err);
			clearerr(PosTxt);
		}
		return -1;
	}
	if(fclose(PosTxt)){perror("J'ai eu tellement de mal a ouvrir le fichier Pixmap.bin que je refuse de le fermer!"); return -1;}
	fprintf(stdout, "J'ai réussi! Ma vie a enfin un sens!\n");
	return 0;
}




int main(int argc, char **argv){
	data_t data;
	pixmap_t Pixels;
	coordinate_t Red = {0,0,0}, Yellow = {0,0,0}, White = {0,0,0};

	if(readArguments(argc, argv, &data)) return -1;

	if(readPixmap(&Pixels)) return -1;

	if(findBalls(&Pixels, &data, &Red, &Yellow, &White)) return -1;

	if(writePosTxt(&Red, &Yellow, &White)) return -1;

	return 0;
}