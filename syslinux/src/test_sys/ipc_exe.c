#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i;

	printf("args:\n");
	for (i = 0; i < argc; ++i)
		printf("argv[%d]: %s\n", i, argv[i]);
	printf("\n");

	i = 10;
	while (i--) {
		printf("hello\n");
		sleep(1);
	}

	return 0;
}
