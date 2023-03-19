## Problem #2

Write a program ***txt2epub*** that given a **list of n text files - f1.txt, f2.txt,...,
fn.txt - on the command line, applies the pandoc command to each of the files generating
EPUB versions.**

The conversion from text to EPUB **must be done in parallel by n processes created for this
purpose**. Each process gets a name from argv[] and converts it as described. After the
processes have finished converting all the files, **the last step of the program txt2epub will
be the generation of a .zip file with the n files in EPUB format**.