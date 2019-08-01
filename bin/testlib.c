#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "NMT_stdlib.h"
int main()
{
    char *file_name = "/tmp/NMT_read_file_unittest.test";
    char *file_content;

    NMT_stdlib_read_file(file_name, &file_content);
    printf("%s\n", file_content);
}
