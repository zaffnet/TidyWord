/*************************	 TIDY WORD	  *************************************
*
*   C PROGRAM TO CLEAN LARGE TEXT FILES SO THAT THEY CAN BE USED FOR FURTHER 
*   WORD PROCESSING AND INFORMATION RETRIEVAL. 
*
*	By: ZAFARULLAH MAHMOOD 		
*	zafarullahmahmood@gmail.com
*
*   ** Removes special characters & insignificant words (less than 3 letters 
*      long).
*   ** Removes stop words (a list may be given).
*   ** Uses  a base  dictionary to  cluster most of the  words to 25,000 odd
*      common English words (uses [6of12 dictionary]
*      (http://wordlist.aspell.net/12dicts-readme/#nof12))
*   ** The output file can be fed to a  Porter's   Stemming Algorithm. This 
*      decreases the    volume of word cloud (i.e. number of distinct words) 
*      as almost 95% of the words are reduced to the base dictionary.
*
*
*   complie: gcc -std=c99 -o tidyword tidyword.c tidy.h 
*   run: ./tidyword input.txt [stop.txt base.txt]  [output.txt]
*	
*	This file is a part of TidyWord GitHub Repository.
*	https://github.com/zafar11235/TidyWord.git
*	Everyone is welcome to contribute and pull requests
*	 
*
*						     GNU GENERAL PUBLIC LICENSE
*	This file is a part of TidyWord
*	TidyWord  is a  free  Software  written  by ZAFARULLAH  MAHMOOD : you  can 
*   redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation, either version  3 of 
*   the License, or (at your option) any later version.
*
*   TidyWord  is distributed in the  hope  that it will be useful, but WITHOUT 
*   ANY  WARRANTY; WITHOUT  EVEN THE  IMPLIED  WARRANTY OF MERCHANTABILITY  or 
*   FITNESS FOR A PARTICULAR PURPOSE.  See  the GNU General Public License for 
*   more details.
*
*  	You should  have received a  copy of the  GNU General Public License along 
*   with the distribution.  If not, see: <http://www.gnu.org/licenses/>.
*
*	Release: 1.0
*	Last Updated: March 2, 2016				                  New Delhi, India
*
******************************************************************************/

// standard libraries
# include <ctype.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// includes
# include "tidy.h"

// measures the number of words deviated from  the base dictionary
int dev = 0;

/* Main Program */
int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        printf("Usage:\n");
        printf("./tidyword input.txt stop.txt base.txt output.txt\n");
        return 1;
    }

	// open the input text file 
    FILE* fp = fopen(argv[1], "r");
    // report and terminate if can not open the text
    if (fp == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    //open the output text file
    FILE* fo = fopen(argv[4], "w");
    // report and terminate if can not open the text
    if (fp == NULL)
    {
        printf("Could not open %s.\n", argv[4]);
        return 1;
    }

    // load the stop words /
    bool stop_loaded = load_stop(argv[2]);
    // report and terminate if cannot open the stop_dict
    if (!stop_loaded)
    {
        printf("Could not load %s.\n", argv[2]);
        return 1;
    }

    //load the base words
    bool base_loaded = load_base(argv[3]);
    // report and terminate if cannot open the stop_dict
    if (!base_loaded)
    {
        printf("Could not load %s.\n", argv[3]);
        return 1;
    }

    //prepare to count words
    int index = 0;
    long long words = 0;
    char word[LENGTH+1];
    char outword[LENGTH+1];

    // print each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphanumerics, apostrophes (e.g., H1N1 virus)
        if (isalnum(c) || (c == '\'' && index > 0))
        {
            // append only alpha char to word
            if(isalpha(c))
            {
                word[index] = tolower(c);
                index++;
            }

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c));
                // prepare for new word
                index = 0;
            }
        } // end if char is alphanumeric

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';
            // check whether the word is a stop word or length <= 2 chars
            if(!check_stop(word) && strlen(word) > 2)
            {

                if (check_base(word, outword) == 0)
                {
                    if (strlen(outword) >= 2 && strlen(outword) <= 4)
                    {
                        fprintf(fo,"%s\n", outword);
                        fprintf(fo,"%s\n", outword);
                        fprintf(fo,"%s\n", outword);
                        fprintf(fo,"%s\n", outword);
                        fprintf(fo,"%s\n", outword);
                        fprintf(fo,"%s\n", outword);

                    }
                    else if (strlen(outword) >= 2)
                    {
                        fprintf(fo,"%s\n", outword);
                        fprintf(fo,"%s\n", outword);
                        fprintf(fo,"%s\n", outword); 
                    }
                }   
                else
                {
                    if (strlen(outword) >= 2)
                        fprintf(fo,"%s\n", outword); 
                }
            }
            //update counter
            words++;
            // prepare for next word
            index = 0;
        }
    }// end for char c till EOF

    // print the last word
    if (index > 0)
    {
        // terminate current word
        word[index] = '\0';
        // check whether the word is a stop word or length <= 2 chars
        if(!check_stop(word) && strlen(word) > 2)
        {

            if (check_base(word, outword) == 0)
            {
                if (strlen(outword) >= 2 && strlen(outword) < 7)
                {
                    fprintf(fo,"%s\n", outword);
                    fprintf(fo,"%s\n", outword);
                    fprintf(fo,"%s\n", outword);
                    fprintf(fo,"%s\n", outword);
                    fprintf(fo,"%s\n", outword);
                    fprintf(fo,"%s\n", outword);

                }
                else if (strlen(outword) >= 2)
                {
                    fprintf(fo,"%s\n", outword);
                    fprintf(fo,"%s\n", outword);
                    fprintf(fo,"%s\n", outword); 
                }
            }
            else
            {
                if (strlen(outword) >= 2)
                    fprintf(fo,"%s\n", outword); 
            }
        }
        //update counter
        words++;
        // prepare for next word
        index = 0;
    }

    // check whether there was an error reading
    if (ferror(fp))
    {
        fclose(fp);
        printf("Error reading %s.\n", argv[1]);
        return 1;
    }

    // check whether there was an error writing
    if (ferror(fo))
    {
        fclose(fp);
        printf("Error reading %s.\n", argv[4]);
        return 1;
    }
    // end the program peacefully
    //print the number of words processed
    printf("NUMBER OF WORDS\t\t%lld\n", words);
    printf("NUMBER OF DEVIATIONS\t%d\n\n", dev);
    // close text
    fclose(fp);
    //unallocate stop trie
    unload_stop();
    //unallocate base trie
    unload_base();
}// end main
