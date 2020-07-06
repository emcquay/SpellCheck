// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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
const unsigned int N = 10000;

//Number of words read into the dictionary (needed to pass a value to a "void function(void)"")
unsigned int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //allocate a node of space to use as a cursor to read the link list
    node *tmp = malloc(sizeof(node));
    //return false if not enough memory to allocate space
    if (tmp == NULL)
    {
        printf("Not enough memory\n");
        return 0;
    }
    //determine the index in the hash table to search by calling the hash function on the word being checked
    int index = hash(word);
    //iterate over the nodes in the linked list setting tmp to the next node
    for (tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        //check if the word being checked matches the word at the current node case insensitive and return true if it does
        if(strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
        else if(strcasecmp(word, tmp->word) > 0)
        {
            return false;
        }
    }
    //if no match is found return false (I am thinking that I could abort sooner since
      //each list should be sorted reverse alphabetically so if the current node is less then the word being check it wont be in the list at all
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int word_len = strlen(word);
    int sum_char = 0;
    for (int c = 0, i = 0, l = word_len; i < l; i++)
    {
        c = tolower(word[i]);
        sum_char += c;
    }
    int index = sum_char % N;
    return index;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //first need to open file and read the first element
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    char new_word[LENGTH + 1];
    //continuously find new words until EOF is reached
    while (fscanf(file, "%s", new_word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("not enough memory,\n");
            return false;
        }
        //copy the new word into the node and set node to point ot NULL
        strcpy(n->word, new_word);
        n->next = NULL;
        //run the new word through the hash function to get an index
        int index = hash(n->word);
        if (table[index] == NULL)
        {
            n->next = NULL;
            table[index] = n;
        }
        else
        {
        //put new node at the start of the appropriate list according to the hash index
        n->next = table[index];
        table[index] = n;
        }
        word_count++;
        //should maybe sort them but to search a list you need to go through each anyway
    }
    printf("%i\n", word_count);
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *cursor = malloc(sizeof(node));
    node *tmp = malloc (sizeof(node));
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    // TODO
    return true;
}
