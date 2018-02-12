#include <stdio.h>
#include <arpa/inet.h>

int main() {
	char a[] = {1,2,3,4};
	int b = *((int *)a);
	printf("a = 0x%x\n", b);
	printf("a = 0x%x\n", htonl(b));
	if (b == 0x01020304)
		printf("big endian\n");
	else
		printf("littie endian\n");

	int c = 18041;
	printf("c = %d\n", c);
	c = htons(c);
	printf("c = %d\n", c);
	return 0;
}
