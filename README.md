# TidyWord

----
## What is TidyWord?

> TidyWord is a free software written in C to clean large text files so that they can be used for further word processing and Information Retrieval. It is fast and efficient and can easily process 70 million words in seconds. The cleaned output file can be easily given to a stemmer or can be used for purposes like word frequency, document distancing, similarity and clustering, tagging, etc.

----
## Usage
* Removes special characters insignificant words (less than 3 letters long).
* Removes stop words (a list may be given).
* Uses a base dictionary to smartly cluster most of the words to 25,000 odd common English words (uses [6of12 dictionary](http://wordlist.aspell.net/12dicts-readme/#nof12))
* The output file can be fed to Porter's Stemming Algorithm. This decreases the volume of word cloud (i.e. number of distinct words) as almost 95% of the words are reduced to the base dictionary.

### To compile and run TidyWord using gcc:
```
gcc -std=c99 -o tidyword tidyword.c tidy.h 
./tidyword input.txt stop.txt base.txt > output.txt
```

For suggestions contact: zafarullahmahmmod@gmail.com
