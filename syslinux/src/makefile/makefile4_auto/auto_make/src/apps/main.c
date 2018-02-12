#include <stdio.h>
#include <stdlib.h>

#include "../../../../makefile4_auto/auto_make/h/sub1/sub1.h"
#include "../../../../makefile4_auto/auto_make/h/sub2/sub2.h"
#include "../../../../makefile4_auto/auto_make/h/sub3/sub3.h"

int main(void) {
	printf("%s, %d: %s()\n", __FILE__, __LINE__, __FUNCTION__);
	sub1(4);
	sub2(5);
	sub3(6);
	return 0;
}
