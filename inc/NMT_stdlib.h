/*NMT_stdlib.h:    Header for library of standard functions for 
 *                 NM Technologies

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

#ifndef _NMT_stdlib_
#define _NMT_stdlib_

//------------------Structs & Enums----------------//
typedef enum {OK, NOK} NMT_result;

//--------------Global Definitions----------------//
extern const char *result_enum2str[] = {"OK","NOK"};

//------------------Prototypes----------------------//
extern void       NMT_stdlib_split(char *string, char *param, char ***item_array, int *no_of_items);
extern int        NMT_stdlib_count(char *string, char *param);
extern size_t     NMT_stdlib_get_file_size(char *filepath);
extern NMT_result NMT_stdlib_read_file(char *filepath, char **file_content);

#endif
