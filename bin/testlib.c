#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "NMT_stdlib.h"
#include "NMT_log.h"

int main()
{
    NMT_result result = OK;

    char *file_name = "/home/nmohan/github/NiRobot/tst/dat/tst_log_settings.json";
    result = NMT_log_init(file_name, true);

    if (result == OK)
    {
        printf("My name is: %s\n", log_settings.out_file_name);
        printf("My name is: %s\n", log_settings.log_level);
        printf("My name is: %s\n", log_settings.log_dir);
        printf("My name is: %d\n", log_settings.log_verbosity);
    }
    

    return 0;

}
