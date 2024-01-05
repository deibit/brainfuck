# Brainfuck

These are some of mine implementations of the Brainfuck programming language in the form of interpreters.

Why?

Why not?

## Python

`python3 brainfuck.py <program>`

## C

`cc brainfuck.c -o bf`

`./bf <program>`

## Test programs

See `programs` folder. 

Not mine, scraped from the Internets.

## Some tech notes

Brainfuck is simple. 

Memory used to be 30.000 cells with byte size initialized to zero.

There is a pointer pointing to one of the cells anytime.

Canonical Brainfuck programs are limited to these operands: "<>+-,.[]"

< > moves the pointer along the memory.

+- increment, decrements the cell values.

. print the character pointed by pointer.

, input (not implemented in my interpreters yet)

[ if memory[pointer] is zero jump to the matching ] position.

] if memory[pointer] is NOT zero return to the matching [ position.

That's all. There are variants: wrapping memory, extended operants, cell size, etc. Almost all unchallenging.
