﻿/*
  Application file

  Find the highest  scoring Scrabble word from a set of seven Scrabble letters
  ----------------------------------------------------------------------------

  Course 04-630 Data Structures and Algorithms for Engineers, Assignment 7

  In the game of Scrabble , a player tries to form a word from seven letters, each letter having an individual value,
  such that the value of the word (i.e. the sum of the individual letter values) is the highest possible.
  This program reads a sequence of 7-letter strings and, for each string, finds the word that has the highest Scrabble score.
  The word must exist in a list of words provided in a separate file.
  The values of each letter are provided in another separate file.

  There are two variants of this problem:

   (a)	to find the highest-scoring 7-letter word (if it exists) or,
   (b)	to find the highest-scoring word of any length.

   Variant (a) simply requires the evaluation of all permutations of the 7 letters and finding the one that gives the highest score (if it exists in the list of words).
   Variant (b) finding the optimal permutation of all subsets of the seven letters.

   Input
   -----
   The first line of the input file will have the filename of a file containing a list of acceptable words. This file is provided.

   The second line of the input file will have the filename of a file containing a list of letter-value pairs, one pair per line. This file is provided.

   There will be one 7-letter string on of the following lines.  These are the Scrabble letters which your program should use to form the highest-scoring word possible.

   Output
   ------
   The first line of your output file contains an Andrew Id.

   For each test case in the input, we output the highest scoring word and its Scrabble value.
   Each result is placed on a separate line.
   We use a field width of 7 for the words and a field width of 2 for the value so that the results are tabulated in an orderly manner.

   For option (a) above, if you cannot find a 7-letter word comprising the given letters in the list of words, we print a value of 0. 

   Sample Input
   ------------
   ../data/words.txt
   ../data/letter_values.txt
   sitnesw
   nbandoa
   nbanboa
   abandoo
   evensaz
   wittnes

   Sample Output – Option (a)
   dvernon
   witness 10
   abandon 10
   ban      5
   band     7
   seven    8
   twin     7

   Sample Output – Option (b)
   dvernon
   witness 10
   abandon 10
   nbanboa  0
   abandoo  0
   evensaz  0
   wittnes  0

   The input is read from a file input.txt and output is written to a file output.txt.

   ****************************************************************************************************************
   *** This version just performs data input and calls the generic backtrack function to print the permutations ***
   *** It needs to be extended to complete the assignment.                                                      ***
   *** The inline comments suggest where these extensions may be required.                                      ***
   ***                                                                                                          ***
   *** It prints the input to the terminal to verify that it has been read correctly.                           ***
   *** If you want the 7! permutations printed to the terminal, set the print_permuations flag to true          ***
   *** in the process_solution() function. This function is located in the scrabbleImplementation.cpp file.     ***
   ****************************************************************************************************************

   David Vernon
   8 April 2019

*/

#include "scrabble.h"

int main() {

	char words_filename[MAX_STRING_LENGTH];              // filename where the list of English words are stored
	char letter_value_filename[MAX_STRING_LENGTH];       // filename where the list of Scrabble letter-values pairs are stored

	struct record_type letter_values[NUMBER_OF_LETTERS]; // list of values associated with each Scrabble tile / letter

	char input_string[MAX_STRING_LENGTH];                // string to store a line from the letter_value file
	char word[MAX_STRING_LENGTH];                        // string to store a word from the dictionary
	char scrabble_letters[NUMBER_OF_TILES + 1];            // string to store the Scrabble letters / tiles in each test case
	int a[NMAX + 1];                                       // permutation of letters

	int i;                                               // general purpose counters
	int n;                                               // number of letters / tiles
	int word_count;                                      // number of words read from the file of words
	int letter_value_count;                              // number of letter-value pairs 
	char letter;                                         // Scrabble letter
	int value;                                           // Scrabble letter value


	/* My changes for newly used variables */
	BINARY_TREE_TYPE wordTree;						   // binary tree to store the dictionary words
	ELEMENT_TYPE e;                                     // holds the element to be inserted into the tree
	struct combWord allPermutations[5041];              // holds all the permutations of the 7 letters
	int permutationsCount = 0;                          // count of the number of permutations
	char currentWordPerm[NUMBER_OF_TILES];              // holds the current permutation of the 7 letters
	struct foundWord foundWords[100];					// holds the words found in the dictionary in an array max of 10 each word has 7 letters
	int countFound = 0;
	int highestFoundIndex;             // holds the highest scoring word




	int debug = false;                                    // flag: if TRUE print information to assist with debugging

	FILE* fp_in;                                         // file pointer for input file
	FILE* fp_out;                                        // file pointer for output file
	FILE* fp_in2;                                        // file pointer for dictionary file
	FILE* fp_in3;                                        // file pointer for letter value pair file
	int end_of_file;                                     // end of file 
	int end_of_file2;                                    // flags

	/* open input and output files */

	if ((fp_in = fopen("../data/input.txt", "r")) == 0) {
		printf("Error can't open input input.txt\n");
		exit(0);
	}

	if ((fp_out = fopen("../data/output.txt", "w")) == 0) {
		printf("Error can't open output output.txt\n");
		exit(0);
	}

	fprintf(fp_out, "jfadiji\n");

	/* read the name of the English words file from the input file    */
	/* then read words and store them in a suitable data structure    */
	/* -------------------------------------------------------------- */

	end_of_file = fscanf(fp_in, "%s", words_filename);  // read a filename

	if (debug) printf("%s\n", words_filename);

	if (end_of_file != EOF) {

		/* open the word list file */

		if ((fp_in2 = fopen(words_filename, "r")) == 0) {
			printf("Error can't open word list file %s\n", words_filename);
			prompt_and_exit(1);
		}

		/* initialize the tree */
		initialize(&wordTree);

		end_of_file2 = fscanf(fp_in2, "%s", word);
		word_count = 1;

		while (end_of_file2 != EOF) {

			// if (debug) printf ("%4d: %s\n",word_count,word);

			for (i = 0; i < (int)strlen(word); i++) { //convert to lower case 
				word[i] = tolower(word[i]);
			}

			/* store the word in some data-structure, e.g. a binary search tree */
			/* -----------------------------------------------------------------*/

			assign_element_values(&e, word);
			insert(e, &wordTree);

			/* get the next word */

			end_of_file2 = fscanf(fp_in2, "%s", word);
			word_count++;
		}

		fclose(fp_in2);
	}

	//print(wordTree); // print the words in the tree

	/* read the name of the letter-value pair file from the input file                         */
	/* then read the list of letter-value pairs and store them in a dictionary data structure  */
	/* --------------------------------------------------------------------------------------- */

	end_of_file = fscanf(fp_in, "%s", letter_value_filename); // read the next filename

	if (end_of_file != EOF) {

		/* open the letter-value pairs file */
		if ((fp_in3 = fopen(letter_value_filename, "r")) == 0) {
			printf("Error can't open letter-value pairs file %s\n", letter_value_filename);
			prompt_and_exit(1);
		}

		/* read the letters and their values */
		/* --------------------------------- */
		fscanf(fp_in3, "%d", &letter_value_count);

		if (debug) printf("Number of letter value pairs %d\n", letter_value_count);

		/* make sure we read no more than the maximum number of letter-value pairs */
		if (letter_value_count > NUMBER_OF_LETTERS) {
			letter_value_count = NUMBER_OF_LETTERS;
		}

		fgetc(fp_in3); // move past end of line for subsequent fgets

		for (i = 0; i < letter_value_count; i++) {

			/* get the letter-value pairs */
			fgets(input_string, MAX_STRING_LENGTH, fp_in3);

			/* extract the symbol */
			sscanf(input_string, "%c%d", &letter, &value); // NB: this fails if we put a space after the %c   !!!

			/* store the letter-value pair in an array of letter-value structures */
			/* it would be better to store them in a letter-value dictionary      */

			letter_values[i].letter = tolower(letter); // convert to lower case
			letter_values[i].value = value;

			if (debug) {
				printf("letter value: %c %2d\n", letter_values[i].letter, letter_values[i].value);
			}
		}
	}


	/* now start processing the list of seven-letter strings from the input file */

	end_of_file = fscanf(fp_in, "%s", scrabble_letters);  // read a set of seven letters as a seven-letter string

	while (end_of_file != EOF) {

		/* convert to lower case */
		n = (int)strlen(scrabble_letters);
		for (i = 0; i < n; i++) {
			scrabble_letters[i] = tolower(scrabble_letters[i]);
		}

		if (debug) {
			printf("Scrabble letters: %s\n", scrabble_letters);
		}

		if (n != NUMBER_OF_TILES) {
			printf("Number of Scrabble letters is not equal to 7\n");
		}

		/* main processing begins here */
		/* --------------------------- */

		countFound = 0; //reset the count of found words
		int wordScore = 0;

		/* use backtracking to generate all permutations and store the permutation giving the highest value */

		backtrack(a, 0, n, allPermutations, &permutationsCount, scrabble_letters, wordTree, &countFound, foundWords, letter_values, fp_out);  // This is the original call from the notes and will just print the permutations to the terminal
		// (set the print_permutations flag to true in the process_solution() function)
		// It needs to be modified so that the data structures required for Assignment 7 are passed as arguments
		// e.g. the seven letters, the letter-value pairs, and the list of admissible words
		// It also needs to be modified so that it returns some data structures, e.g. the optimum permutation.
		// The required data structures depend on which variant of Assignment 7 is being implemented:
		// the best seven-letter permutation or the best permutation of all sub-sets of the seven letters.

/* now process the result here ... */

		//at the end of backtracking function, the foundWords array will have all the valid words found in the dictionary
		//the countFound will have the number of valid words found in the dictionary


		/* print the results */

		/* if no valid word is found, print the scrabble letters and a score of 0 */
		if (countFound == 0) {
			printf("%s		%d\n", scrabble_letters, 0);
			fprintf(fp_out, "%s		%d\n", scrabble_letters, 0);
		}
		else {
			/* for a valid word, obtain the score based on the letter values and based on the one found first alphabetically */
			highestFoundIndex = getHighestWordindex(foundWords, countFound);

			printf("%s		%d\n", foundWords[highestFoundIndex].word, foundWords[highestFoundIndex].score);
			fprintf(fp_out, "%s		%d\n", foundWords[highestFoundIndex].word, foundWords[highestFoundIndex].score);
		}

		//reset all the values
		permutationsCount = 0;
		countFound = 0;
		wordScore = 0;

		//reset found words
		for (int i = 0; i < 20; i++) {
			strcpy(foundWords[i].word, "");
			foundWords[i].score = 0;
		}

		//reset a which is the permutation of letters
		for (int i = 0; i < NUMBER_OF_TILES; i++) {
			a[i] = 0;
		}


		end_of_file = fscanf(fp_in, "%s", scrabble_letters);  // read the next set of seven letters
	}

	fclose(fp_in);
	fclose(fp_in2);
	fclose(fp_in3);
	fclose(fp_out);

	if (debug) {
		prompt_and_exit(1);
	}

}
