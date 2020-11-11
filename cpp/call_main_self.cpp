#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
//#include <time.h>
 
int main() {
    printf("main\n");
    sleep(1);
    main();
    return 0;
}
