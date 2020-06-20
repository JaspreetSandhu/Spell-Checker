// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

//
int nodeCount = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);

    node *tmp = table[index];

    while (tmp != NULL) {

        if (strcasecmp(tmp->word, word) == 0) {
            return true;
        }

        tmp = tmp->next;
    }

    return false;
}

// Hashes word to a number
// Adapted from https://stackoverflow.com/questions/2571683/djb2-hash-function
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    for (const char* ptr = word; *ptr != '\0'; ptr++)
    {
     hash = ((hash << 5) + hash) + tolower(*ptr);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL) {
        return false;
    }

    char *word = malloc(LENGTH);

    while (fscanf(file, "%s", word) != EOF) {

        node *tmpNode = malloc(sizeof(node));

        if (tmpNode == NULL) {
            return false;
        }

        strcpy(tmpNode->word, word);
        int index = hash(word);

        node *current = table[index];
        tmpNode->next = current;
        table[index] = tmpNode;

        nodeCount++;
    }

    free(word);
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return nodeCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++) {

        node *list = table[i];

        node *cursor = list;
        node *tmp = cursor;

        while (tmp != NULL) {
            cursor = tmp->next;
            free(tmp);
            tmp = cursor;
        }

    }

    return true;
}
