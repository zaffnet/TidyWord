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
    printf("COSINE SIMILARITY\t%.4f\n", cosine_sim);

    /*double angle = acos(cosine_sim)*57.2958;
    printf("ANGLE\t\t\t%.2f DEGREES\n",angle);*/
    
    double jaccard_sim = jaccard();
    printf("JACCARD SIMILARITY\t%f\n", jaccard_sim);
}// end main
