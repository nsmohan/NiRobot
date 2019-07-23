/*NMT_stdlib.c:    Library of standard functions for 
 *                 NM Technologies

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

NMT_result NMT_stdlib_read_file(char *filepath, char *file_content)
{
    //Input     : File Path, file_content pointer
    //Output    : Return OK/NOK Status
    //Function  : Read file and populate file_content with contents

    //Initialize Variables
    FILE *fp;
    int c; 
    int index = 0;
    NMT_result result = OK;
    
    //Main Part of Function
    if (access(filepath, F_OK) != -1)
    {
        fp = fopen(filepath, "r");
        while((c = fgetc(fp)) != EOF)
        {
            file_content[index] = c;
            index++;
        }
        file_content[index] = '\0';
        return result;
    }
    else
    {
        result = NOK;
        return result;
    }
}

void NMT_stdlib_split(char *string, char *param, char ***item_array, int *no_of_items)
{
    //Input     : String and the value(s) to split from
    //Output    : Array of of split strings and the number of strings
    //Function  : Split string based on param(s) passed and return split array

    //Initialize Variables
    char *str;
    char *token;
    int  i = 0;

    //Get Number of items in array
    *no_of_items = NMT_stdlib_count(string, param) + 1;

    //Memory Allocation
    *item_array = (char **)malloc(sizeof(char *) * *no_of_items);
    str = malloc(sizeof(string));
    
    //Main part of the function
    strcpy(str, string);
    token = strtok(str, param);
    while(token != NULL)
    {
        (*item_array)[i] = (char *)malloc(sizeof(char) * sizeof(token));
        strcpy((*item_array)[i++], token);
        token = strtok(NULL, param);
    }

    //Free Allocated Memory
    free(str);
}

int NMT_stdlib_count(char *string, char *param)
{
    //Input     : String and the value to look for in the string 
    //Output    : Number of occurances of param in the string
    //Function  : Count number of occurences of a parameter in the string passed

    //Initialize Variables
    int counter;
    int no_of_matches = 0;


    //Main section of the function
    for (int i = 0; param[i] != '\0'; i++)
    {
        counter = 0;
        for (int ii = 0; string[ii] != '\0'; ii++)
        {
            if (string[ii] == param[i])
            {
                counter++;
            }
        }
        no_of_matches = no_of_matches + counter;
    }
    
    //Exit the function
    return no_of_matches;
}

