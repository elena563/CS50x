// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 78;

// Hash table
node *table[N];

// count words in dictionary
int counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int index = hash(word);
    node *cursor = table[index];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
        return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int hash = 0;
    int i = 0;

    while (word[i] != '\0')
    {
        hash += (i + 1) * tolower(word[i]);
        i++;
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open file");
        return false;
    }

    char w[LENGTH +1];

    while (fscanf(dict, "%s", w) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(dict);
            return false;
        }

        strcpy(new_node->word, w);
        int index = hash(w);
        new_node->next = table[index];
        counter += 1;
        table[index] = new_node;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i<N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
