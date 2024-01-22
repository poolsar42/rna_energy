#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// I need this structure to store the distances distribution
// Dynamic array structure
typedef struct
{
    float *array;
    size_t used;
    size_t size;
} DynamicArray;

// Initialize the dynamic array
void initArray(DynamicArray *a, size_t initialSize)
{
    a->array = (float *)malloc(initialSize * sizeof(float));
    a->used = 0;
    a->size = initialSize;
}

// Add an element to the dynamic array
void insertArray(DynamicArray *a, float element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

// Prints all elements of the dynamic array
void printArray(DynamicArray *a)
{
    for (int i = 0; i < a->used; i++)
    {
        printf("%f ", a->array[i]);
    }
    printf("\n");
}

// Free the dynamic array
void freeArray(DynamicArray *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
