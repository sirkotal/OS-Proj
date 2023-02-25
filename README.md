# SO-Proj

Operating Systems (SO) project - coding exercises in C utilizing processes, pipes and Linux terminal commands/system calls.

**Grade: 5.9/6**

## Problem #1

Write a program ***samples*** that given **one text file and two integers, n and m**, on
the command line, **prints a sequence of n text fragments, each with m characters from the
original file. Fragments must be obtained in n different positions** in the file at random and
be written sequentially in stdout without any separator or line change, and between > and
< characters.

## Problem #2

Write a program ***txt2epub*** that given a **list of n text files - f1.txt, f2.txt,...,
fn.txt - on the command line, applies the pandoc command to each of the files generating
EPUB versions.**

The conversion from text to EPUB **must be done in parallel by n processes created for this
purpose**. Each process gets a name from argv[] and converts it as described. After the
processes have finished converting all the files, **the last step of the program txt2epub will
be the generation of a .zip file with the n files in EPUB format**.

## Problem #3

Write a program ***tokenring*** that **gets 3 integers - n, p and t - from the command
line**. When executed, **it creates n processes connected to each other by "named pipes"**. The
named pipes are designated pipe1to2, pipe2to3,..., pipento1 and **each allows one-way
communication between the i-th and the i+1-th processes. The last named pipe closes the
ring allowing process n to communicate with process 1**. Once this process ring is created,
**p1 should send a token (a message with an integer with an initial value 0) to the next
process (p1 > p2) and so on (p2 > p3 >... > pn > p1 >...). The token must circulate
between processes endlessly, increasing its value at each hop. Each time a process receives
the token, it must immediately resend it to the next process or, with a probability of p,
block its submission during t seconds and print a message alerting to this fact. In either case, the value of the token must be incremented.**
