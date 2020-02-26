#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PM.h"

int main(int argc, char **argv) {

	unsigned int w,h;

	if (argc==3) {
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	} else {
		w = myW;
		h = myH;
	}

	FILE *BinFile;
	BinFile = fopen("Pixmap.bin", "w");
	fwrite(&w, sizeof(unsigned int), 1, BinFile);
	fwrite(&h, sizeof(unsigned int), 1, BinFile);
	printf("%d\n", fwrite(myPM, sizeof(unsigned int), myH*myW, BinFile));


	return 0;
}











