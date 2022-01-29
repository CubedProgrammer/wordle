# wordle
Inspired by https://www.powerlanguage.co.uk/wordle/
### Installation
GCC
```sh
gcc -O3 -c load.c main.c usrget.c
gcc -static -o wordle load.o main.o usrget.o
```
CLang
```sh
clang -O3 -c load.c main.c usrget.c
clang -static -o wordle load.o main.o usrget.o
```
MSVC
```sh
cl /Fewordle /O2 load.c main.c usrget.c
```
You'll need a reference dictionary file called fivedict.txt, you might find one in the releases tab.

If you wish to change the number of guesses, you can pass in a single number as a command line argument, the number of guesses.
### Game Rules
You get six guesses to guess a five letter word.
You may only guess words that are present in the dictionary.
After a word is guessed, the colour of the letters are as follows.
#### Letter Colours
For each letter in the guessed word...

If the target word has the same letter in the same position, that letter shall be green.

Otherwise, if that letter appears at least once in the target, it shall be yellow.

Otherwise, it shall be red.
