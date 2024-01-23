#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// How to compile file with glib
// gcc training.c `pkg-config --cflags --libs glib-2.0`
#include <glib.h>

// Custom headers
#include "./headers/read_file.h"
#include "./headers/dynamic_array.h"

int main(int argc, char *argv[])
{
    FILE *fptr;
    // argv[1] - path to the input file, e.g. ./data/1E95 Pseudoknot Structure.pdb
    // argv[2] - path to the output file, e.g. ./pseudo_energy_1E95.csv

    // Open a file in read mode
    fptr = fopen(argv[1], "r");

    // Store the content of the file
    char myString[100];

    int initial_size = 100;
    // Dynamically allocated array of subset of strings from the file
    char **array_of_strings = malloc(initial_size * sizeof(char *));
    const char *subStringATOM = "ATOM";
    const char *subStringC = "C3'";

    //  ---- READ THE FILE ----
    // Number of strings currently stored
    int *current_size = (int *)malloc(sizeof(int));
    *current_size = 0;
    // to split string by space
    const char delim[2] = " ";
    char *token1;
    char *token2;

    readFile(fptr,
             myString,
             delim,
             token1,
             token2,
             subStringC,
             subStringATOM,
             array_of_strings,
             current_size,
             initial_size);

    //  ---- COUNT THE DISTANCES ----
    // Initialize the hash-map
    // Create a new GLib hash table
    GHashTable *hashTable = g_hash_table_new(g_str_hash, g_str_equal);

    // Initialize basepairs of interest
    char *basepairs[10] = {
        "AA",
        "AU",
        "AC",
        "AG",
        "UU",
        "UC",
        "UG",
        "CC",
        "CG",
        "GG",
    };

    for (int j = 0; j < 10; j++)
    {
        // Insert empty dynamic array to all basepairs in hash table
        DynamicArray *a = malloc(sizeof(DynamicArray));
        initArray(a, 1);
        g_hash_table_insert(hashTable, basepairs[j], a);
    }

    // Compute the distances
    for (int i = 0; i < *current_size; i++)
    {
        char *copyFstStr = malloc(strlen(array_of_strings[i]) + 1);
        strcpy(copyFstStr, array_of_strings[i]);
        float x1, y1, z1;
        char bp1, bp2;
        token1 = strtok(copyFstStr, delim);
        int index1 = 0;
        while (token1 != NULL)
        {

            token1 = strtok(NULL, delim);
            index1++;
            if (index1 == 3)
            {
                bp1 = token1[0];
            }
            if (index1 == 6)
            {
                x1 = atof(token1);
            }
            if (index1 == 7)
            {
                y1 = atof(token1);
            }
            if (index1 == 8)
            {
                z1 = atof(token1);
            }
        }
        // Only if there're at least 3 nucleotides distance
        for (int j = i + 3; j < *current_size; j++)
        {
            char *copySndStr = malloc(strlen(array_of_strings[i]) + 1);
            strcpy(copySndStr, array_of_strings[j]);
            float x2, y2, z2;

            token2 = strtok(copySndStr, delim);

            int index2 = 0;
            float distance;
            while (token2 != NULL)
            {
                token2 = strtok(NULL, delim);
                index2++;
                if (index2 == 3)
                {
                    bp2 = token2[0];
                }
                if (index2 == 6)
                {
                    x2 = atof(token2);
                }
                if (index2 == 7)
                {
                    y2 = atof(token2);
                }
                if (index2 == 8)
                {
                    z2 = atof(token2);
                }
            }
            free(copySndStr);
            char *key = malloc(2 * sizeof(char));
            key[0] = bp1;
            key[1] = bp2;
            // Add the distance to the array of distances between bp1 and bp2 in hash table
            // if the bp1+bp2 is not in the table, then insert it to bp2+bp1
            DynamicArray *a = g_hash_table_lookup(hashTable, key);
            if (a == NULL)
            {
                key[0] = bp2;
                key[1] = bp1;
                a = g_hash_table_lookup(hashTable, key);
            }
            distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
            insertArray(a, distance);
        }
        free(copyFstStr);
    }

    // COMPUTE FREQUENCIES
    // OBSERVED FREQUENCIES
    GHashTable *frequency_distribution = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTableIter iterFreq;
    int ref_count = 0; // we'll need to use it for reference count
    gpointer key, value;
    g_hash_table_iter_init(&iterFreq, hashTable);
    while (g_hash_table_iter_next(&iterFreq, &key, &value))
    {
        // Allocate a new frequencies array for each key
        float *frequencies = malloc(20 * sizeof(float));
        if (frequencies == NULL)
        {
            // Handle memory allocation failure
            exit(1);
        }

        // Initialize the new frequencies array
        for (int i = 0; i < 20; i++)
        {
            frequencies[i] = 0.;
        }

        DynamicArray *a = (DynamicArray *)value;
        ref_count += a->used;
        for (int i = 0; i < a->used; i++)
        {
            int b = (int)a->array[i];
            if (b >= 20)
            {
                frequencies[19] += 1. / a->used;
            }
            else
            {
                frequencies[b] += 1. / a->used;
            }
        }

        g_hash_table_insert(frequency_distribution, (char *)key, frequencies);
    }

    // REFERENCE FREQUENCIES

    float ref_frequencies[20] = {
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
        0.,
    };

    g_hash_table_iter_init(&iterFreq, hashTable);
    while (g_hash_table_iter_next(&iterFreq, &key, &value))
    {
        DynamicArray *a = (DynamicArray *)value;
        for (int i = 0; i < a->used; i++)
        {
            int b = (int)a->array[i];
            if (b >= 20)
            {
                ref_frequencies[19] += 1. / ref_count;
            }
            else
            {
                ref_frequencies[b] += 1. / ref_count;
            }
        }
    }

    // Compute Score (Pseudo-Energy), log-ratio
    GHashTable *pseudo_energy = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTableIter iterScore;
    g_hash_table_iter_init(&iterFreq, frequency_distribution);
    while (g_hash_table_iter_next(&iterFreq, &key, &value))
    {
        float *freqs = (float *)value;
        float *energy = malloc(20 * sizeof(float));
        if (energy == NULL)
        {
            // Handle memory allocation failure
            exit(1);
        }

        // Initialize the new frequencies array
        for (int i = 0; i < 20; i++)
        {
            if (freqs[i] != 0.)
            {
                energy[i] = -log(freqs[i] / ref_frequencies[i]);
            }
            else
            {
                energy[i] = 0.;
            }
        }

        // g_hash_table_insert(frequency_distribution, (char *)key, frequencies);
        g_hash_table_insert(pseudo_energy, (char *)key, energy);
    }

    g_hash_table_to_csv(pseudo_energy, argv[2]);

    // Free the memory
    GHashTableIter iter;

    g_hash_table_iter_init(&iter, hashTable);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        DynamicArray *a = (DynamicArray *)value;
        free(a->array); // Free the internal array
        free(a);        // Free the DynamicArray structure itself
    }
    g_hash_table_destroy(hashTable);

    g_hash_table_iter_init(&iter, frequency_distribution);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        float *frequencies = (float *)value;
        free(frequencies);
    }
    g_hash_table_destroy(frequency_distribution);

    g_hash_table_iter_init(&iter, pseudo_energy);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        float *energies = (float *)value;
        free(energies);
    }
    g_hash_table_destroy(pseudo_energy);

    // free the memory
    // Finally, free the array of strings if they were dynamically allocated
    for (int i = 0; i < *current_size; i++)
    {
        free(array_of_strings[i]); // Free each string
    }
    free(array_of_strings);
    free(current_size);

    // Close the file
    fclose(fptr);
}
