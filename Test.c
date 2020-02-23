#include <stdio.h>

struct test {
	int x;
	int y;
};

struct test temp = {1, 2};

int main(){
	temp = {3, 4};
	printf("%d, %d", temp.x, temp.y);

    return 0;
}