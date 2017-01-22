/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (value < 0)
    {
        return false;
    }
    int ind1 = 0;
    int ind2 = n-1;
    while (ind2 - ind1 + 1 > 0)
    {
        int mid = (ind2 - ind1 + 1) / 2 + ind1;
        if (values[mid] == value)
        {
            return true;
        }
        else if (values[mid] > value)
        {
            ind2 = mid - 1;
            
        }
        else
        {
            ind1 = mid + 1;
            
        }
        
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int limit = 65536;
    int arr[limit];
    for (int i = 0; i < limit; i++)
    {
        arr[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        arr[values[i]] += 1;
    }
    int count = 0;
    for (int i = 0; i < limit; i++)
    {
        if (arr[i] != 0)
        {
            for (int j = 0; j < arr[i]; j++)
            {
                values[count] = i;
                count++;
            }
        }
    }
    
    return;
}
