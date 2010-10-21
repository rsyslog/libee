#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i = 0;
	while(i < atoi(argv[1])) {
		printf("%d\n", i++);
	}
	return 1;
}
