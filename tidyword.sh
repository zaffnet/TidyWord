#!/bin/bash
echo $1
echo $2

if [ $# -eq 0 ]; then
    echo "Usage:"
    echo "./tidyword input1.txt input2.txt"
    exit 1
fi

gcc -std=c99 -o tidy tidyword.c tidy.h

echo "Tidying first Input File...."
./tidy $1 stop.txt base.txt tidyoutput1.txt
OUT=$?
if [ $OUT -eq 1 ];then
   exit;
fi

echo "Tidying second Input File...."
./tidy $2 stop.txt base.txt tidyoutput2.txt
OUT=$?
if [ $OUT -eq 1 ];then
   exit;
fi

gcc -std=c99 -o porter porter.c

echo "Stemming first Input File...."
./porter tidyoutput1.txt > stemmedoutput1.txt
OUT=$?
if [ $OUT -eq 1 ];then
   exit;
fi
echo "Done"
echo " "

echo "Stemming second Input File...."
./porter tidyoutput2.txt > stemmedoutput2.txt
OUT=$?
if [ $OUT -eq 1 ];then
   exit;
fi
echo "Done"
echo " "

cat stemmedoutput1.txt | tr [:space:] '\n' | grep -v "^\s*$" | sort | uniq | sort -bnr > arrangedoutput1.txt
cat stemmedoutput2.txt | tr [:space:] '\n' | grep -v "^\s*$" | sort | uniq | sort -bnr > arrangedoutput2.txt

echo "Calculating Document Similarity..."
echo " "
gcc -std=c99 -o docsim docsim.c docsim.h -lm
./docsim arrangedoutput1.txt arrangedoutput2.txt

echo " "