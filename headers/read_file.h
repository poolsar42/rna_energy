#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void readFile(FILE *fptr,
              char *myString,
              char *delim,
              char *token1,
              char *token2,
              char *subStringC,
              char *subStringATOM,
              char **array_of_strings,
              int *current_size,
              int initial_size)
{
    while (fgets(myString, 100, fptr))
    {
        if ((strstr(myString, subStringC) != NULL) && (strstr(myString, subStringATOM) != NULL))
        {

            // Get the first token

            int index1 = 0;
            int index2 = 0;
            // Walk through other tokens

            // In my file I have multiple models (because of NMR)
            // This is the way I select only first model from the file
            if (*current_size >= 1)
            {
                char *copyPrevStr = (char *)malloc(strlen(array_of_strings[*current_size - 1]) + 1);
                strcpy(copyPrevStr, array_of_strings[*current_size - 1]);
                token1 = strtok(copyPrevStr, delim);
                while (index1 != 5)
                {
                    token1 = strtok(NULL, delim);
                    index1++;
                }
                char *copyCurStr = (char *)malloc(strlen(myString) + 1);
                strcpy(copyCurStr, myString);
                token2 = strtok(copyCurStr, delim);
                while (index2 != 5)
                {
                    token2 = strtok(NULL, delim);
                    index2++;
                }
                // If token went down - it means the algo switched to the second model
                // in this case I want to break the reading -> I have all necessary data
                if (atoi(token1) > atoi(token2))
                {
                    break;
                }
                free(copyPrevStr);
                free(copyCurStr);
            }
            array_of_strings[*current_size] = strdup(myString);
            printf("%s", array_of_strings[*current_size]);

            *current_size = *current_size + 1;
        }
        // Add the string to the array

        if (*current_size == initial_size)
        {
            initial_size *= 2;
            char **temp = (char **)realloc(array_of_strings, initial_size * sizeof(char *));
            array_of_strings = temp;
        }
        // Read the content and print it
    }
}

void g_hash_table_to_csv(GHashTable *table, const char *filename)
{
    // Open the file for writing
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Cannot open file %s for writing\n", filename);
        return;
    }

    // Write header line for CSV (optional, if you want column titles)
    fprintf(fp, "Key,Frequency1,Frequency2,...,Frequency20\n");

    // Iterate over the hash table
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        // Assuming the key is a string and value is a float array
        char *str_key = (char *)key;
        float *frequencies = (float *)value;

        // Write the key
        fprintf(fp, "%s", str_key);

        // Write the frequencies
        for (int i = 0; i < 20; i++)
        {
            fprintf(fp, ",%f", frequencies[i]);
        }
        fprintf(fp, "\n"); // Newline at the end of each row
    }

    // Close the file
    fclose(fp);
}