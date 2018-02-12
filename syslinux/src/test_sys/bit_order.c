#include <stdio.h>
#include <stdint.h>

//downlink rlc data block header of dacch
typedef struct bit_order_s {
	uint8_t b5 :5, b1 :1, b2 :2;
} bit_order_t;

void print_bit0(uint8_t c) {
	int i;
	for (i = 0; i < 8; ++i) {
		int tmp = 1 << (7 - i);
		if ((c & tmp) == tmp)
			printf("1 ");
		else
			printf("0 ");
	}
	printf("\n");
}

void print_bit(uint8_t c) {
	int i;
	for (i = 0; i < 8; ++i)
		printf("%u ", (c << i) >> 7);
	printf("\n");
}
int main() {
	bit_order_t aa;
	aa.b5 = 1;
	aa.b1 = 1;
	aa.b2 = 2;
	printf("b5: %u, b1: %u, b2: %u\n", aa.b5, aa.b1, aa.b2);

	printf("aa: %u\n", *((uint8_t *) &aa));
	print_bit0(*((uint8_t *) &aa));
	printf("sizeof(aa) = %d\n", sizeof(aa));
	return 0;
}
