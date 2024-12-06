/*

   scrabbleImplementation.cpp - implementation file for a program named scrabble
   ============================================================================

   This program is for Assignment No. 4, Course 04-630 Data Structures and Algorithms for Engineers.

   Functions that facilitate the execution of the program are defined here:
   1. function to prompt the user to enter a key after successfully running the program
   2. function to compare the input word against the dictinary tree
   3. function to compute the height of a binary tree
   4. function to compute the size of a binary tree
   5. function to compute the number of probes to traverse a binary tree
   6. function to print the words in the binary tree in a formatted manner

   Please refer to the application file - assignment4Application.cpp - for details of the functionality of this program


   File organization
   -----------------

   scrabble.h                        interface file:
										contains the declarations required to use the functions that implement the solution to this problem
										typically, these will include the definitions of the abstract data types and data structures used in
										the implementation

   scrabbleImplementation.cpp        implementation file:
										contains the definitions of the functions that implement the algorithms used in the implementation

   scrabbleApplication.cpp           application file:
										contains the code that instantiates the abstract data types and calls the associated functions
										in order to effect the required functionality for this application

   binaryTree.h                         interface file:
										This is an external header file that contains the code that instantiates the abstract data types and
										calls the associated functions for an abstract data type which is a linked list

   binaryTreeImplementation.cpp         implementation file:
										contains the code that instantiates the abstract data types and calls the associated functions
										in order to effect the required functionality of a linked list.

   Author
   ------

   Joseph Fadiji, MSIT '26, Carnegie Mellon University Africa
   05/12/2024

   Audit Trail
   ----------


*/

#include "scrabble.h"

int debug = false;                                    // flag: if TRUE print information to assist with debugging


void transformWord(char* originalWord, int a[], char* transformedWord, int k) {
	for (int i = 1; i <= k; i++) { // Assuming `a` is 1-indexed
		transformedWord[i - 1] = originalWord[a[i]];
	}
	transformedWord[k] = '\0'; // Null-terminate the string
}

/* Custom function to check if the input word is present in the dictionary i.e valid */
ELEMENT_TYPE* compareWords(char* inputWord, BINARY_TREE_TYPE tree) {

	if (tree == NULL) {
		// Base case: If the tree is empty, return NULL
		return NULL;
	}

	char* treeWord = (char*)malloc((strlen(tree->element.string) + 1) * sizeof(char));
	//char treeWord[strlen(tree->element.string) + 1];

	if (treeWord == NULL) {
		return NULL;
	}

	strcpy(treeWord, tree->element.string);

	for (int k = 0; k < strlen((tree->element.string) + 1); k++) {
		treeWord[k] = tolower(treeWord[k]);
	}

	int comparison = strcmp(inputWord, treeWord);

	//if (comparison == 0) {
	//	// Exact match found
	//	return &tree->element;
	//}

	free(treeWord);

	if (comparison > 0) {
		// Recursively search in the left subtree
		//printf("Going left because %s comes before %s\n",inputWord, tree->element.string);
		return compareWords(inputWord, tree->left);
	}
	else if (comparison < 0) {
		// Recursively search in the right subtree
		//printf("Going right because %s comes after %s\n",inputWord, tree->element.string);
		return compareWords(inputWord, tree->right);
	}
	else {
		// If the words are equal, return the found element
		//printf("\nWord found\n");
		return &(tree)->element;
	}
}

/* Function to obtain word score using letter values */
int getWordScore(char* word, struct record_type letter_values[], int letter_count) {
	int score = 0;

	// Iterate through each character in the word
	for (int i = 0; i < strlen(word); i++) {
		char currentLetter = word[i];

		// Look up the value for the current letter in letter_values
		for (int j = 0; j < letter_count; j++) {
			if (letter_values[j].letter == currentLetter) {
				score += letter_values[j].value;  // Add the value to the score
				break;  // Stop searching once a match is found
			}
		}
	}

	return score;
}

void backtrack(int a[], int k, int input, combWord allPerm[], int* permutation, char* originalWord, BINARY_TREE_TYPE dictionaryTree, int* countFound, struct foundWord foundWords[], struct record_type letter_values[], FILE* fp_out) {
	int c[MAXCANDIDATES];     /* candidates for next position  */
	int ncandidates;          /* next position candidate count */
	int i;                    /* counter                       */

	if (is_a_solution(a, k, input)) {
		process_solution(a, k, input, allPerm, permutation);
	}
	else {
		k = k + 1;
		construct_candidates_permutation(a, k, input, c, &ncandidates);

		for (i = 0; i < ncandidates; i++) {
			a[k] = c[i];
			char transformedWord[NMAX];
			transformWord(originalWord, a, transformedWord, k);

			/*int wordComparison = strcmp(transformedWord, 'band');*/

			//fprintf(fp_out, "%s\n", transformedWord);

			// Check if current partial or complete word exists in dictionary
			// This allows checking words of all lengths during backtracking
			if (compareWords(transformedWord, dictionaryTree) != NULL) {
				// Word exists, add to found words if not a duplicate
				bool isDuplicate = false;
				for (int j = 0; j <= *countFound; j++) {
					if (strcmp(foundWords[j].word, transformedWord) == 0) {
						isDuplicate = true;
						break;
					}
				}

				if (!isDuplicate) {
					(*countFound)++;
					strcpy(foundWords[*countFound].word, transformedWord);
					int wordScore = getWordScore(transformedWord, letter_values, NUMBER_OF_LETTERS);
					foundWords[*countFound].score = wordScore;
					if (debug) printf("Transformed Word: %s\n", transformedWord);
					if (debug) printf("Word Score: %d\n", foundWords[*countFound].score);
				}
			}

			// Continue backtracking regardless of whether word is found
			backtrack(a, k, input, allPerm, permutation, originalWord, dictionaryTree, countFound, foundWords, letter_values, fp_out);
		}
	}
}



bool is_a_solution(int a[], int k, int n) {
	return (k == n);
}


void process_solution(int a[], int k, int input, combWord allPermutations[], int* permutationCount) {

	int i;                       /* counter */
	bool print_permuations;      /* flag ... set to true if you want the permutations listed to the terminal */

	print_permuations = false;

	if (print_permuations) {
		for (i = 1; i <= k; i++) {
			printf(" %d", a[i]);
		}
		printf("\n");
	}

	/* pass current permutation to allPermutation array */
   // Create a new combWord to store the current permutation
	combWord currentPermutation;

	// Copy the elements from a[] to the current permutation
	// Note: Assuming a[] starts from index 1, so we adjust the copying accordingly
	for (i = 1; i <= k; i++) {
		currentPermutation.word[i - 1] = a[i];
	}

	// Add the current permutation to the allPermutations array
	allPermutations[*permutationCount] = currentPermutation;

	// Increment the permutation count
	*permutationCount = *permutationCount + 1;


}



void construct_candidates_subset(int a[], int k, int n, int c[], int* ncandidates) {
	c[0] = TRUE;
	c[1] = FALSE;
	*ncandidates = 2;
}



void construct_candidates_permutation(int a[], int k, int input, int c[], int* ncandidates) {
	bool in_perm[NMAX] = { false };

	// Mark indices already used in the current partial solution
	for (int i = 1; i < k; i++) {
		in_perm[a[i]] = true;
	}

	*ncandidates = 0;
	for (int i = 0; i < input; i++) {
		if (!in_perm[i]) {
			c[(*ncandidates)++] = i;
		}
	}
}



/* Function to get the highest word score in the found words array */
int getHighestWordindex(struct foundWord foundWords[], int foundWordCount) {
	int highestScore = 0;
	int highestIndex = 0;


	//first sort the words in the array alphabetically
	for (int i = 1; i <= foundWordCount; i++) {
		for (int j = i + 1; j <= foundWordCount; j++) {
			if (strcmp(foundWords[i].word, foundWords[j].word) > 0) {
				struct foundWord temp = foundWords[i];
				foundWords[i] = foundWords[j];
				foundWords[j] = temp;
			}
		}
	}


	// Iterate through the found words array
	for (int i = 1; i <= foundWordCount; i++) {
		if (foundWords[i].score > highestScore) {
			highestScore = foundWords[i].score;  // Update the highest score
			highestIndex = i;
		}
	}



	return highestIndex;
}



void prompt_and_exit(int status) {
	printf("Press any key to continue and close terminal\n");
	getchar();
	exit(status);
}