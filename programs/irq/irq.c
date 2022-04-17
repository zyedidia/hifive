#include "libfive/uart.h"
#include "libfive/exception.h"

void exception(unsigned cause) {
    printf("exception!\n");
}

int main() {
    set_exception_handler(exception);

    volatile int* x = NULL;
    *x = 10;

    return 0;
}
