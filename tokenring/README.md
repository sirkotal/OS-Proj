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