#include "../../makefile21/h/interface.h"

int main(){
	printf("%s, %d: %s\n", __FILE__, __LINE__, __FUNCTION__);
	sub1();
	sub2();
	return 0;
}
