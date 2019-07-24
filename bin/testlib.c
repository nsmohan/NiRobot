#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "NMT_stdlib.h"
int main()
{
    char **item_array;
    int no_of_items;

    NMT_stdlib_split("Nitin-is-the-king-of-the-world-and he is going to be the master of c", "- ", &item_array, &no_of_items);

    for (int i = 0; i < no_of_items; i++)
    {
        printf("%s\n", item_array[i]);
    }
    return 0;
}
