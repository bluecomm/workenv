#include "../../../../makefile4_auto/auto_make/h/sub1/sub1.h"

int sub1(int num){
	printf("%s, %d: %s(): num = %d\n", __FILE__, __LINE__, __FUNCTION__, num);
	return 0;
}
