#include "write_log.h"

int main() {
    WRITE_FORMATTED_LOG("%d variable argument", 1);
    WRITE_FORMATTED_LOG("%d variable %s", 2, "arguments");
    return 0;
}

