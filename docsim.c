/*************************   TIDY WORD    *************************************
*
*   C PROGRAM TO CLEAN LARGE TEXT FILES SO THAT THEY CAN BE USED FOR FURTHER 
*   WORD PROCESSING AND INFORMATION RETRIEVAL. 
*
*   By: ZAFARULLAH MAHMOOD      
*   zafarullahmahmood@gmail.com
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
*   This file is a part of TidyWord GitHub Repository.
*   https://github.com/zafar11235/TidyWord.git
*   Everyone is welcome to contribute and pull requests
*    
*
*                            GNU GENERAL PUBLIC LICENSE
*   This file is a part of TidyWord
*   TidyWord  is a  free  Software  written  by ZAFARULLAH  MAHMOOD : you  can 
*   redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation, either version  3 of 
*   the License, or (at your option) any later version.
*
*   TidyWord  is distributed in the  hope  that it will be useful, but WITHOUT 
*   ANY  WARRANTY; WITHOUT  EVEN THE  IMPLIED  WARRANTY OF MERCHANTABILITY  or 
*   FITNESS FOR A PARTICULAR PURPOSE.  See  the GNU General Public License for 
*   more details.
*
*   You should  have received a  copy of the  GNU General Public License along 
*   with the distribution.  If not, see: <http://www.gnu.org/licenses/>.
*
*   Release: 1.0
*   Last Updated: March 7, 2016                               New Delhi, India
*
******************************************************************************/


// standard libraries
# include <ctype.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include "docsim.h"

/* Main Program */
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage:\n");
        printf("./docsim input1.txt input2.txt\n");
        return 1;
    }

    // load the words from input file 1 
    bool loaded = load(argv[1], argv[2]);
    // report and terminate if cannot open the stop_dict
    if (!loaded)
    {
        printf("Could not load files.\n");
        return 1;
    }

    double euclid_distance = euclid();
    printf("EUCLIDIAN DISTANCE:\t%f\n", euclid_distance);

    double cosine_sim = cosine();
    printf("COSINE SIMILARITY:\t%.4f\n", cosine_sim);

    /*double angle = acos(cosine_sim)*57.2958;
    printf("ANGLE\t\t\t%.2f DEGREES\n",angle);*/
    
    double jaccard_sim = jaccard();
    printf("JACCARD SIMILARITY:\t%f\n", jaccard_sim);

    double pearson_dist = pearson();
    printf("PEARSON CORR DIST:\t%f\n", pearson_dist);

    /*double avgkl_dist = avgkl();
    printf("AVG KL DIST:\t\t%f\n", avgkl_dist);*/
}// end main
