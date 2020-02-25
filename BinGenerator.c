#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PM.h"

int main(){
	FILE *BinFile;
	BinFile = fopen("Pixmap.bin", "w");
	fwrite(&myW, sizeof(unsigned int), 1, BinFile);
	fwrite(&myH, sizeof(unsigned int), 1, BinFile);
	fwrite(myPM, sizeof(unsigned int), myH*myW+2, BinFile);




	return 0;
}











