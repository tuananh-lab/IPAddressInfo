#include <stdio.h>
#include <stdlib.h>
#include "Network_Info.h"

int main() {
    char *info = getAllInterfaceIPAddressInfo();
    printf("%s", info);
    free(info);
    return 0;
}
