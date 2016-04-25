// -ggdb3


/// Author name
#define AUTHOR "alartum"
/// Project name
#define PROJECT "List Optimized"
/// Version
#define VERSION "1"
#define NO_OUTPUT
//#undef NO_OUTPUT
#define DEBUG
#undef DEBUG
#define NDEBUG

#include <stdio.h>
#include "mylib.h"
#include "list_t.h"
#include "hash_functions.h"
#include <limits.h>
#include <time.h>
#include <ctype.h>
#define OK_PUNCT "-'"
// Prime values are better
#define HASH_TABLE_SIZE 997

//void load_dict (const char string[],ListHead)

list_node_t* hash_table_get_node (const list_t* hash_table,
                                  const size_t size,
                                  const char word[])
{
    size_t position;
    position = hash_xor_opt(word) % size;

    return list_t_search(hash_table + position, word);
}

int main (int argc, char* argv[])
{
    clock_t begin, end;
    double time_spent;

    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Default part BEGIN
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    INFO();
    CHECK_DEFAULT_ARGS();
    char inName[NAME_MAX] = {}, outName[NAME_MAX] = {};
    switch (argc)
    {
    case 3:
        strcpy (inName, argv[1]);
        strcpy (outName, argv[2]);
        break;
    case 2:
        strcpy (inName, argv[1]);
        strcpy (outName, "info.csv");
        break;
    default:
        WRITE_WRONG_USE();
    }
    // Words will be loaded here
    Buffer words = {};
    if (!buffer_construct(&words, inName))
        return WRONG_USE;
    // We'll store lists here
    list_t* lists = (list_t*)calloc (HASH_TABLE_SIZE, sizeof(list_t));
        for (int i = 0; i < HASH_TABLE_SIZE; i++)\
            list_t_construct(lists + i, 0);
    //buffer_dump(&words);
    size_t counter = 0, total = words.length;
    #ifndef NO_OUTPUT
    printf ("Progress:\n");
    #endif // NO_OUTPUT
    // Get the chars array from buffer

    begin = clock();
    for (char *word = words.chars; counter < words.length; word++){
        #ifndef NO_OUTPUT
        if (counter % (size_t)total / 1000){
            printf ("\r%.1f%%", 100.0*counter/total);
            fflush(stdout);
        }
        #endif // NO_OUTPUT
        counter++;
        // Locating first letter
        if (isalnum (*word)){
            //Pointer to the word
            char *new_word = word;
            // Finding first non-letter
            while (isalnum (*word) || strchr(OK_PUNCT, *word)){
                word++;
                counter++;
            }
            //Indicating the end
            *word = '\0';
            //printf ("Trying to add: %s\n", new_word);
            // Finding the right place to insert
            size_t position;
            position = hash_xor_opt(new_word) % HASH_TABLE_SIZE;
            list_t_add(lists + position, new_word);
            //list_t_dump(lists + position);
        }
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf ("Hash table creation: %lfms\n", time_spent*1000);
    // Newline after progress bar
    printf ("\n");
    /// Some tests
    /*
    size_t position;
    position = hash_xor_opt("herdersleven") % HASH_TABLE_SIZE;
    list_t_dump(lists + position);
    list_node_t_remove(lists[position].storage + 3);
    list_t_dump(lists + position);
    list_t_exchange_num(lists+position, 2,4);
    list_t_dump(lists + position);
    */
    /// TIME MEASUREMENT
    begin = clock();
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf ("Empty timer: %lfms\n", time_spent*1000);

    list_node_t* found = NULL;
    begin = clock();
    found = hash_table_get_node (lists, HASH_TABLE_SIZE, "draagberries");
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf ("Search time: %lfms\n", time_spent*1000);
    if (found)
        list_node_t_dump(found);
    else
        printf ("Not found");

    /// TIME MEASUREMENT END
    #ifndef NO_OUTPUT
    open_file (out, outName, "w", "#Output error");
    fprintf (out, "Hash value, ");
        for (size_t i = 0; i < HASH_TABLE_SIZE; i++)
            fprintf (out, "%lu, ", i);
    fprintf (out, "\n");

    fprintf (out, "Hash type: " "xor_opt" ", ");
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++){
        fprintf (out, "%lu, ", lists[i].size);
        list_t_destruct(lists + i);
    }
    fprintf (out, "\n");
    close_file(out);
    #endif // NO_OUTPUT

    printf ("\nThe info has been successfully written to %s.\n", outName);
    free (lists);
    buffer_destruct(&words);

    return NO_ERROR;
}
