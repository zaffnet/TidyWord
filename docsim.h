// standard libraries
# include <ctype.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

// trie struct
typedef struct node
{
    bool word;
    int length;
    struct node* children[26];
    int occur1;
    int occur2;
}node;


// implement trie for words
node* trie = NULL;

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 55

#define THREHSHOLD 2000

// intermediate variables
double euclid_sum = 0.0000;
double cosine_sum1 = 0.0000;
double cosine_sum2 = 0.0000;
double cosine_prod = 0.0000;

//document lengths
int doc_len1 = 0;
int doc_len2 = 0;

// threshold for document terms
int terms1 = 0;
int terms2 = 0;

// prototypes
bool load(const char* text1, const char* text2);
void insert(const char* word, int swt);
void euclid_traverse(node* ptr);
double euclid(void);
int hash(char c);
bool unload(void);
void finish(node* ptr);
void cosine_traverse(node* ptr);
double cosine(void);
double jaccard(void);
double len_normalize(int word_length);

/*  Loads text1 and text2 as mentioned in argv[2] and 
**	argv[3] of main
** -> text1 is the name of the file containg words
**	  from first doc
** -> returns true if loading is success else false
*/
bool load(const char* text1, const char* text2)
{     
    FILE* f1 = fopen(text1,"r");
    if(f1 == NULL)
    {
        return false;
    }
    FILE* f2 = fopen(text2,"r");
    if(f2 == NULL)
    {
        return false;
    }

    char word[LENGTH+1];
    int swt = 0;
    while (fscanf(f1, "%s", word) != EOF)
    {   

        insert(word, 0);
        doc_len1++;  
    }
    swt = 1;
    while (fscanf(f2, "%s", word) != EOF)
    {   
        insert(word, 1); 
        doc_len2++;  
    }
    fclose(f1);
    fclose(f2);
    return true;
}

/*
** Inserts words to trie
**/
void insert(const char* word, int swt)
{
    if (swt == 0 && terms1 > THREHSHOLD)
        {
            return;
        }
    if (swt == 1 && terms2 > THREHSHOLD)
    {
        return;
    }
    //Instantiate the trie if NULL
    if (trie == NULL)
    {
        trie = malloc(sizeof(node));
    }
    // Assign a pointer to the trie
    node* ptr = trie;
    for (int i = 0; i < strlen(word); i++)
    {
        int h = hash(word[i]);
        if(ptr->children[h] == NULL)
        {
            ptr->children[h] = malloc(sizeof(node));
            ptr->children[h]->word = false; 
            ptr->children[h]->occur1 = 0;
            ptr->children[h]->occur2 = 0;
        }
        ptr = ptr->children[h];    
    }
    ptr->word = true;
    ptr->length = strlen(word);
    if(swt == 0)
    {
        if (ptr->occur1 == 0) terms1++;
    	ptr->occur1  = ptr->occur1 + 1;
        
    }
    else
    {
        if (ptr->occur2 == 0) terms2++;
    	ptr->occur2 = ptr->occur2 + 1;
    }
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

void euclid_traverse(node* ptr)
{
	if (ptr->word == true)
	{
        // tf is the number of documents containing the term
        int tf = (ptr->occur1 > 0 && ptr->occur2 >0) ? 2: 1;
		euclid_sum += pow((ptr->occur1*log(2/tf) - ptr->occur2*log(2/tf)), 2);
	}
    for(int i = 0; i < 26; i++)
    {
        if(ptr->children[i] == NULL)
            ;
        else
            euclid_traverse(ptr->children[i]);
    }

}

double euclid(void)
{
    euclid_sum = 0;
    euclid_traverse(trie);
    return sqrt(euclid_sum);
}


void cosine_traverse(node* ptr)
{
    if (ptr->word == true)
    {
        //int tf = (ptr->occur1 > 0 && ptr->occur2 > 0) ? 2: 1;
        double term1 = (ptr->occur1)*1.0000/doc_len1*len_normalize(ptr->length);
        double term2 = (ptr->occur2)*1.0000/doc_len2*len_normalize(ptr->length);
        cosine_sum1 += pow(term1, 2);
        cosine_sum2 += pow(term2, 2);
        cosine_prod += term1*term2;
    }
    for(int i = 0; i < 26; i++)
    {
        if(ptr->children[i] == NULL)
            ;
        else
            cosine_traverse(ptr->children[i]);
    }

}

double cosine(void)
{
    cosine_sum1 = 0.0000;
    cosine_sum2 = 0.0000;
    cosine_prod = 0.0000;
    cosine_traverse(trie);
    double x = (sqrt(cosine_sum1)*sqrt(cosine_sum2));
    if (x == 0)
    {
        return 1;
    }
    else
    {
        return cosine_prod/x;
    }
    
}

double jaccard(void)
{
    double x = (cosine_sum1 + cosine_sum2 - cosine_prod);
    if(x == 0)
    {
        return 1;
    }
    else
    {
            return (cosine_prod)/x;
    }
}


bool unload(void)
{
    finish(trie);
    return true;
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

double len_normalize(int word_length)
{
    switch(word_length)
    {
        case 0: return 0;
        case 1: return 0.0009;
        case 2: return 0.02302;
        case 3: return 0.1604;
        case 4: return 0.2425;
        case 5: return 0.3664;
        case 6: return 0.4637;
        case 7: return 0.5000;
        case 8: return 0.5238;
        case 9: return 0.6149;
        case 10: return 0.7057;
        case 11: return 0.7995;
        case 12: return 0.8714;
        case 13: return 0.9210;
        case 14: return 0.9582;
        default: return 1.0000;
    }
}