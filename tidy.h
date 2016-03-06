/*************************   TIDY WORD    *************************************
*
*   HEADER FILE: IMPLEMENTS THE FUNCTIONALITY BEHIND TIDYWORD 
*   (See tidyword.c)
*
*   By: ZAFARULLAH MAHMOOD      
*   zafarullahmahmood@gmail.com
*
*   
*   Release: 1.0
*   Last Updated: March 3, 2016                               New Delhi, India
*
******************************************************************************/

// standard libraries
# include <ctype.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// trie struct
typedef struct node
{
    bool word;
    struct node* children[26];
}node;

// implement trie for stop_dict
node* stop_trie = NULL;

// implement trie for base_dict 
node* base_trie = NULL;

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 55

#define DEPTH 0.75

// measures the number of words deviated from  the base dictionary
extern int dev;

// prototypes
bool load_stop(const char* stop_dict);
void insert_stop(const char* word);
bool check_stop(const char* word);
bool unload_stop(void);
bool load_base(const char* base_dict);
void insert_base(const char* word);
int hash(char c);
int check_base(const char* word, char* outword);
int hash(char c);
void finish(node* ptr);


/* Loads stop dictionary as mentioned in argv[2] of main
** -> stop_dict is the name of the file containg stop_words
** -> returns true if loading is success else false
*/
bool load_stop(const char* stop_dict)
{     
    FILE* fp = fopen(stop_dict,"r");
    if(fp == NULL)
    {
        return false;
    }
    char word[LENGTH+1];
    while (fscanf(fp, "%s", word) != EOF)
    {   
        insert_stop(word);  
    }
    fclose(fp);
    return true;
}

/*
** Insert words from stopwords.txt to stop_trie
**/
void insert_stop(const char* word)
{
    //Instantiate the trie if NULL
    if (stop_trie == NULL)
    {
        stop_trie = malloc(sizeof(node));
    }
    // Assign a pointer to the trie
    node* ptr = stop_trie;
    for (int i = 0; i < strlen(word); i++)
    {
        int h = hash(word[i]);
        if(ptr->children[h] == NULL)
        {
            ptr->children[h] = malloc(sizeof(node));
            ptr->children[h]->word = false; 
        }
        ptr = ptr->children[h];    
    }
    ptr->word = true;
}

/*
** Returns true if the given word is present in stop_trie
*/
bool check_stop(const char* word)
{   
    int length = strlen(word);
    node* ptr = stop_trie;
    for(int i = 0; i < length; i++)
    {
        char c = tolower(word[i]);
        int h = hash(c);
        if(ptr->children[h] == NULL)
        {
            return false;
        }
        ptr = ptr->children[h];    
    }
    return ptr->word;
}

/* 
** Helper function to unallocate the stop_trie
*/
bool unload_stop(void)
{
    finish(stop_trie);
    return true;
}

/*
** Returns an int for every char that is present in words so that they 
** can be easily manipulated by children array of node trie
*/
int hash(char c)
{
    if(isalpha(c) != 0)
        return c - 97;
    else
        return 0;
}

/*
** Dellocates, i.e. frees the memory used to implement the trie
*/
void finish(node* ptr)
{
    for(int i = 0; i < 26; i++)
    {
        if(ptr->children[i] == NULL)
            ;
        else
            finish(ptr->children[i]);
    }
    free(ptr);
}

/* Loads base dictionary as mentioned in argv[3] of main
** -> base_dict is the name of the file containg base_words
** -> returns true if loading is success else false
*/
bool load_base(const char* base_dict)
{     
    FILE* fp = fopen(base_dict,"r");
    if(fp == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];
    while (fscanf(fp, "%s", word) != EOF)
    {   
        insert_base(word);  
    }
    fclose(fp);
    return true;
}

/*
** Insert words from base.txt to base_trie
**/
void insert_base(const char* word)
{
    //Instantiate the trie if NULL
    if (base_trie == NULL)
    {
        base_trie = malloc(sizeof(node));
    }
    // Assign a pointer to the trie
    node* ptr = base_trie;
    for (int i = 0; i < strlen(word); i++)
    {
        int h = hash(word[i]);
        if(ptr->children[h] == NULL)
        {
            ptr->children[h] = malloc(sizeof(node));
            ptr->children[h]->word = false; 
        }
        ptr = ptr->children[h];    
    }
    ptr->word = true;
}

/*
** Returns true if the given word is present in base_trie
** Needs special attention as the efficiency and correctness of
** this  function  decides how  successful we are in clustering 
** the text to base dictionary
**    It replaces the word  by a word in  base dictionary  that 
**    is 75% (or whatever DEPTH is)similar to a base word, else 
**    returns the original word.
*/
int check_base(const char* word, char* outword)
{   
    int length = strlen(word);
    node* ptr = base_trie;
    int j = 0;

    // iterate over the chars of word
    for(int i = 0; i < length; i++)
    {
        // if a word falls in the way, mark it down
        if(ptr->word == true)
        {
            j = i;
        }
        char c = tolower(word[i]);
        int h = hash(c);

        // if there is no way down the trie
        if(ptr->children[h] == NULL)
        {
            // if the word ends with s drop it
            if(i + 1 == strlen(word) && word[i] == 's')
            {
                strncpy(outword, word, i);
                outword[i] = '\0';
                return 1;
            }

            // if word is DEPTH*100 % similar 
            if((1.00*i)/strlen(word) >= DEPTH)
            {
                // check for only those word that is just one letter
                // bigger than the marked deviation
                // i.e., inconsistenc|ies -> inconsistenc|y
                for(int k = 0; k < 26; k++)
                {
                    if (ptr->children[k] != NULL )
                    {
                        bool flag = true;
                        for(int l = 0; l < 26; l++)
                        {
                            if (ptr->children[k]->children[l] != NULL)
                                flag = false;
                        }
                        if(flag)
                        {
                            strncpy(outword, word, i);
                            outword[i] = k + 97;
                            outword[i+1] = '\0';
                            return 1;
                        }

                    }
                }
            }
            // else check whether the previously marked word is 
            // DEPTH*100% similar
            // informational -> information
            if((1.00*j)/strlen(word) >= DEPTH)
            {
                strncpy(outword, word, j);
                outword[j] = '\0';
                return 1;
            }
            // if the word seems to be junk ignore it
            /*else if ( j == 0)
            {
                strncpy(outword, word, 1);
                outword[1] = '\0';
                return;
            }*/
            // just copy the original word (no effect)
            else
            {
                strncpy(outword, word, strlen(word));
                outword[strlen(word)] = '\0';
                dev++;
                return 0;
            }

        }

        // if ptr->children[h] is not NULL
        // move one step down the trie
        ptr = ptr->children[h];    
    }

    // We have reached the end of original word

    // if the word is present in base dictionary
    if (ptr->word)
    {
        strncpy(outword, word, strlen(word));
        outword[strlen(word)] = '\0';
        return 1;
    }

    // if another word up the trie is DEPTH*100 similar
    else if((1.00*j)/strlen(word) >= DEPTH)
    {
        strncpy(outword, word, j);
        outword[j] = '\0';
        return 1;
    }
    // if the word seems to be junk
    /*else if ( j == 0)
    {
        strncpy(outword, word, 1);
        outword[1] = '\0';
        return;
    }
    */
    // else just return the original word
    else
    {
        strncpy(outword, word, strlen(word));
        outword[strlen(word)] = '\0';

        dev++;
        return 0;

    }
}

/* 
** Helper function to unallocate the base_trie
*/
bool unload_base(void)
{
    finish(base_trie);
    return true;
}