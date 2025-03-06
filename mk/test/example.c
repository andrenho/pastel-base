#include <stdio.h>

#include "hello.txt.h"

int main()
{
    printf("Imported: %s\n", (const char *) hello_txt);
}