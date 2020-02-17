/** 
 *  @file      NMT_stdlib.c
 *  @brief     Standard Library functions for NMT Technologies
 *  @details   Standard Facilities provided by NMT Technologies
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

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

NMT_result NMT_stdlib_read_file(char *filepath, char **file_content)
{
    /*!
     *  @brief     Read a file and store contents in buffer
     *  @param[in] filepath
     *  @param[out] file_content
     *  @return    NMT_result
     */

    //Initialize Variables
    size_t file_size = 0;
    int index = 0;
    FILE *fp;
    NMT_result result = OK;

    /* Get the file Size */
    file_size = NMT_stdlib_get_file_size(filepath);

    /* Store contents in a temp location */
    char temp_file_content[file_size + 1];


    //Main Part of Function
    if (access(filepath, F_OK) != -1)
    {
        fp = fopen(filepath, "r");
        index = fread(temp_file_content, sizeof(char), file_size, fp);
        temp_file_content[index] = '\0';
    }
    else
    {
        result = NOK;
    }

    //Allocate Memory and store to contents to actual location
    *file_content = (char *)malloc((sizeof(char) * strlen(temp_file_content)) + 1);
    strcpy(*file_content, temp_file_content);

    //Close the file and Exit the Function
    fclose(fp);
    return result;
}

void NMT_stdlib_write_file(char *filepath, char *file_content)
{
    /*!
     *  @brief     Standard function for writing to a file
     *  @param[in] filepath
     *  @param[out] file_content
     *  @return    NMT_result
     */

    //Initialize Variables
    FILE *fp;

    //Main Part of function
    fp = fopen(filepath, "a");
    fprintf(fp, "%s\n", file_content);
    fclose(fp);
}

void NMT_stdlib_split(char *string, char *param, char ***item_array, int *no_of_items)
{
    /*!
     *  @brief     Function to split string based on given parameter(s)
     *             and store in to an array
     *  @param[in] string
     *  @param[in] param
     *  @param[out] item_array
     *  @param[out] no_of_items
     *  @return    NMT_result
     */

    //Initialize Variables
    char *str;
    char *token;
    int  i = 0;

    //Get Number of items in array
    *no_of_items = NMT_stdlib_count(string, param) + 1;

    //Memory Allocation
    *item_array = (char **)malloc(sizeof(char *) * *no_of_items);
    str = (char *)malloc(sizeof(char) * strlen(string) + 1);
    
    //Main part of the function
    strcpy(str, string);
    token = strtok(str, param);
    while(token != NULL)
    {
        (*item_array)[i] = (char *)malloc(sizeof(char) * strlen(token) + 1);
        strcpy((*item_array)[i++], token);
        token = strtok(NULL, param);
    }

    //Free Allocated Memory
    *no_of_items = i;
    free(str);
}

int NMT_stdlib_count(char *string, char *param)
{
    /*!
     *  @brief     Count the number of occurances of a character
     *             in a string
     *  @param[in] string
     *  @param[in] param
     *  @return    no_of_matches
     */

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
                counter++;
        }
        no_of_matches = no_of_matches + counter;
    }
    
    //Exit the function
    return no_of_matches;
}

size_t NMT_stdlib_get_file_size(char *filepath)
{

    /*!
     *  @brief     Get the size of the file in bytes
     *  @param[in] filepath
     *  @return    size
     */

    //Initialize Variables
    size_t size; 
    FILE *fp;

    //Main Part of the function
    fp = fopen(filepath, "r");
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    //Close the file and exit the function
    fclose(fp);
    return size;
}
