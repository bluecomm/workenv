#include <stdio.h>
#include <string.h>
#include <stdint.h>

static char buf[64];
typedef struct tt_s {
	int a;
	char b[0];
	int c;
} tt_t;

extern int32_t print_memory(uint8_t *msg_p, uint32_t len);

int main_test_0_array() {
	char *b = "12345";
	char *ptr = buf;
	((tt_t *) ptr)->a = 1;
	memcpy(((tt_t *) ptr)->b, b, strlen(b));
	ptr+=strlen(b);
	((tt_t *) ptr)->c = 2;

	printf("sizeof(tt_t) = %d\n",sizeof(tt_t));
	print_memory((uint8_t *)buf, (uint32_t)(sizeof(tt_t) + strlen(b)));

	return 0;
}
