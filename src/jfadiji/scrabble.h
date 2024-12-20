/* 
  Interface file

  Find the highest highest scoring Scrabble word from a set of seven Scrabble letters
  -----------------------------------------------------------------------------------

  Course 04-630 Data Structures and Algorithms for Engineers, Assignment 7


  This problem reduces to finding the optimal permutation of all subsets of the seven letters.


  David Vernon
  19 March 2019
*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <limits.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define NUMBER_OF_TILES   7
#define NUMBER_OF_LETTERS 26
#define MAXCANDIDATES     NUMBER_OF_TILES
#define NMAX              NUMBER_OF_TILES+1 // NMAX must be the number of elements in the permutation + 1 to allow for counting from 1, rather than 0. DV 22/3/2019
#define MAX_STRING_LENGTH 81

#include "binaryTree.h"

/* data structure to represent a Scrabble tile */

struct record_type {
   int  value;    // an integer that will be used in the permutation instead of the letter
   char letter;   // the Scrabble letter
};

struct combWord {
	int word[NUMBER_OF_TILES];
};

struct foundWord {
	char word[NUMBER_OF_TILES];
	int score;
};

/* function prototypes */
/* ------------------- */

/* utility function to remove a newline character that may have been included when parsing the input data */

void backtrack            (int a[], int k, int n, combWord allCombination[], int *permutationCount, char* originalWord, BINARY_TREE_TYPE dictionaryTree, int* countFound, struct foundWord foundWords[], struct record_type letter_values[], FILE* fp_out);                                    // original backtrack

bool is_a_solution(int a[], int k, int n);

void construct_candidates_permutation(int a[], int k, int n, int c[], int *ncandidates);          // permutation construct_candidates

void construct_candidates_subset(int a[], int k, int n, int c[], int *ncandidates);               // subset construct_candidates

void process_solution            (int a[], int k, int n, combWord allPermutations[], int *permutationCount);                                         // original process_solution

void prompt_and_exit(int status);
void transformWord(char* originalWord, int* permutation, char* transformedWord, int wordLength);
ELEMENT_TYPE* compareWords(char* inputWord, BINARY_TREE_TYPE tree);
int getWordScore(char* word, struct record_type letter_values[], int letter_count);
int getHighestWordindex(struct foundWord foundWords[], int foundWordCount);
//void backtrack_subset(int a[], int k, int input, combWord allSubsets[], int* subsetCount, char* originalLetters);