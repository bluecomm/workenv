#include "stdio.h"
#include "unistd.h"

int main() {
    fprintf(stdout,"hello ");
    fprintf(stderr,"world ");
    return 0;
}

