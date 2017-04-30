# Formal Languages and Automata : PDA Implementation 
Push Down Automata implementation as a recursive function. It takes all possibilities. 

---
### Information and Requirements

This project was a group project but I was responsible of the implementation and approach.

Language : C

Compiler : gcc

Compile and run : gcc pda.c && ./a.out

Required files : grammer.txt and words.txt

Since it is C, I wrote everything of implementation, such as stacks etc. Thus, the code could seems long but the main part is just the process() function.
And of course it is not my best, because of the deadline and other projects.

Thanks to Merve for homework pdf file.

### Simple information about implementation
The approach I implemented is similar to maze decision stack approach. It takes all possibilities until find a solution. 

As initially, I push the 'S' to stack. Every time I call the process function, the algorithm pop the first non-terminal from stack and push the all possible rules from that non-terminal. After that I copy the current state, steps and output of the process and call again the process.


Current State is a char stack that contains non-terminals mostly.

Steps, just the steps found at that moment.

Output is a char stack that contains terminals from the leftmost.


The implementation is a recursive function, therefore it can process length of 10-11 characters.

This is not an optimal solution, for other solutions you could look my other 2 repositories which is CYK and Divide & Conquer.

### Inputs and Output
grammer.txt
```
) ( + 0 1 *
S E
1 S -> ( E )
2 S -> E + E
3 S -> E * E
4 E -> S
5 E -> 0
6 E -> 1
```
words.txt
```
(0+1)+1
(1+11)
0*(1)
((((1*1+1*1+1+1))))
((0+1*1))
```

output.txt

```
YES
2 4 1 4 2 5 6 6
NO
YES
3 5 4 1 6
YES
1 4 1 4 1 4 1 4 3 6 4 3 4 2 6 6 4 2 6 4 2 6 6
YES
2 4 1 4 2 5 6 6
NO
YES
3 5 4 1 6
YES
1 4 1 4 1 4 1 4 3 6 4 3 4 2 6 6 4 2 6 4 2 6 6
YES
1 4 1 4 3 4 2 5 6 6
YES
2 4 1 4 2 5 6 6
NO
YES
3 5 4 1 6
YES
1 4 1 4 1 4 1 4 3 6 4 3 4 2 6 6 4 2 6 4 2 6 6
YES
1 4 1 4 3 4 2 5 6 6
```

The numbers shows step number as leftmost derivation.