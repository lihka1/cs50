/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


#include "dictionary.h"


typedef struct node
    {
        bool isword;
        struct node *character[27];
    }
    node;
node *root;

// remember no of word and dictionary loaded or not
int words;
bool loadd = false;

// prototype for freeing data structure
void release(node *ptr);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *trav = root;
    // for each letter in the word
    for (int i = 0, n = strlen(word); i< n; i++)
    {
        if (word[i] == '\'')
        {
            if (trav->character[26] == NULL)
            {
                return false;
            }
            else
            {
                trav = trav->character[26];
            }
        }
        else
        {
            if (trav->character[tolower(word[i]) - 'a'] != NULL)
            {
                    trav = trav->character[tolower(word[i]) - 'a'];
            }
            else
            {
                return false;
            }
        }
        
    }
    // check whether the isword is true
    if (trav->isword == true)
    {
        return true;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // count from start
    words = 0;
    
    // start from root
    root = malloc(sizeof(node));
    
    // open dictionay file
    FILE *file = fopen(dictionary, "r");
    
    // also fscanf ead string of maximum len 45 +1 is for '/0'
    char word[LENGTH + 1];
    
    while (fscanf(file, "%s", word) != EOF)
    {
        node *trav = root;
        for (int i = 0, n = strlen(word); i <= n; i++)
        {
            if (word[i] == '\'')
            {
                if (trav -> character[26] == NULL)
                {
                    node *next = malloc(sizeof(node));
                    trav -> character[26] = next;
                    trav = next;
                }
                else
                {
                    trav = trav -> character[26];
                }
            }
            else if (word[i] == '\0')
            {
                words++;
                (trav -> isword) = true;
            }
            else
            {
                if ((trav -> character[word[i] - 'a']) == NULL)
                {
                    node *next = malloc(sizeof(node));
                    trav -> character[word[i] - 'a'] = next;
                    trav = next;
                }
                else
                {
                    trav = trav -> character[word[i] - 'a'];
                }
            }
        }
    }
    
    // close the file...not done is error check
    fclose(file);
    
    // loadd is true
    loadd = true;
    
    // thats all
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (loadd)
    {
        return words;
    }
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node *trav = root;
    release(trav);
    return true;
}

void release(node *ptr)
{
    node *parent = ptr;
    // recursively release the node...back-tracking
    for (int i = 0; i < 27; i++)
    {
        if (ptr->character[i] != NULL)
        {
            release(ptr->character[i]);
        }
    }
    free(parent);
}

