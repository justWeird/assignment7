/*

  Implementation file

  Find the highest highest scoring Scrabble word from a set of seven Scrabble letters
  -----------------------------------------------------------------------------------

  Course 04-630 Data Structures and Algorithms for Engineers, Assignment 7

  This problem reduces to finding the optimal permutation of all subsets of the seven letters.


  David Vernon
  19 March 2019
*/

#include "scrabble.h"

/* original backtracking code ... needs to be adapted for the assignment */

// Function to transform the original word based on a permutation
//void transformWord(char* originalWord, int* permutation, char* transformedWord, int wordLength) {
//	// Iterate through the permutation array
//
//	int originalIndex = 0;
//
//	for (int i = 0; i <= wordLength; i++) {
//		// The permutation array contains 1-based indices
//		// So we subtract 1 to convert to 0-based indices for array access
//		
//		originalIndex = permutation[i] - 1;
//
//		// Copy the character from the original word to the transformed word
//		// based on the permutation
//		if (originalIndex < 0 || originalIndex >= wordLength) {
//			//printf("Index out of bounds\n");
//			continue;
//		}
//		transformedWord[i - 1] = originalWord[originalIndex];
//
//	}
//
//	transformedWord[wordLength] = '\0';
//
//}

void transformWord(char* originalWord, int a[], char* transformedWord, int k) {
	for (int i = 1; i <= k; i++) { // Assuming `a` is 1-indexed
		transformedWord[i - 1] = originalWord[a[i]];
	}
	transformedWord[k] = '\0'; // Null-terminate the string
}




/* Custom function to check if the input word is present in the dictionary i.e valid */

//ELEMENT_TYPE* compareWords(char* inputWord, BINARY_TREE_TYPE tree) {
//	if (tree == NULL) return NULL;
//
//	// Convert both words to lowercase for comparison
//	char inputLower[NMAX];
//	char treeLower[NMAX];
//
//	strcpy(inputLower, inputWord);
//	strcpy(treeLower, tree->element.string);
//
//	for (int i = 0; inputLower[i]; i++)
//		inputLower[i] = tolower(inputLower[i]);
//
//	for (int i = 0; treeLower[i]; i++)
//		treeLower[i] = tolower(treeLower[i]);
//
//	int comparison = strcmp(inputLower, treeLower);
//
//	if (comparison == 0)
//		return &(tree->element);
//
//	if (comparison < 0)
//		return compareWords(inputWord, tree->left);
//
//	return compareWords(inputWord, tree->right);
//}

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


//void backtrack(int a[], int k, int input, combWord allPerm[], int* permutation, char* originalWord, BINARY_TREE_TYPE dictionaryTree, int* countFound, struct foundWord foundWords[], struct record_type letter_values[]) {
//
//	int c[MAXCANDIDATES];     /* candidates for next position  */
//	int ncandidates;          /* next position candidate count */
//	int i;                    /* counter                       */
//
//	if (is_a_solution(a, k, input)) {
//		process_solution(a, k, input, allPerm, permutation);
//	}
//	else {
//		k = k + 1;
//		construct_candidates_permutation(a,k,input,c,&ncandidates); 
//		//construct_candidates_subset(a, k, input, c, &ncandidates);
//		for (i = 0; i < ncandidates; i++) {
//			a[k] = c[i];
//
//			char transformedWord[NMAX];
//
//			transformWord(originalWord, a, transformedWord, k);
//
//			// Check if the current partial permutation matches the file
//			if (compareWords(transformedWord, dictionaryTree) == NULL) {	//if the word isn't there, carryon the permutation
//				//make_move(a, k, input);
//				backtrack(a, k, input, allPerm, permutation, originalWord, dictionaryTree, countFound, foundWords, letter_values);
//				//unmake_move(a, k, input);
//			}
//			else {		//the word exists. Add it to the list and ensure its not a duplicate
//				// Check if the word is already in the foundWords array
//				bool isDuplicate = false;  // Flag to check for duplicates
//				for (int i = 0; i <= *countFound; i++) {
//					if (strcmp(foundWords[i].word, transformedWord) == 0) {
//						isDuplicate = true;  // Found a duplicate
//						break;
//					}
//				}
//
//				if (!isDuplicate) {
//					// If no duplicate, add the word to foundWords
//					(*countFound)++;
//
//					// If no duplicate, add the word to foundWords
//					strcpy(foundWords[*countFound].word, transformedWord);
//
//					int wordScore = getWordScore(transformedWord, letter_values, NUMBER_OF_LETTERS);
//
//					foundWords[*countFound].score = wordScore;
//
//					// Add the word to the binary tree (not shown here)
//					printf("Transformed Word: %s\n", transformedWord);
//					printf("Word Score: %d\n", foundWords[*countFound].score);
//
//
//				}
//
//			}
//		}
//	}
//}
//

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


			// Key changes start here
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
					printf("Transformed Word: %s\n", transformedWord);
					printf("Word Score: %d\n", foundWords[*countFound].score);
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


//void construct_candidates_permutation(int a[], int k, int n, int c[], int* ncandidates) {
//
//	int i;                       /* counter */
//	bool in_perm[NMAX];          /* who is in the permutation? */
//	for (i = 1; i < NMAX; i++) {     /* NB: NMAX must be the number of elements in the permutation + 1 to allow for counting from 1, rather than 0 */
//		in_perm[i] = FALSE;
//	}
//
//	/* original Skiena code is incorrect ... a[i(==0)] is not a valid element of the permutation since we start storing them at a[1]
//
//	   for (i=0; i<k; i++) in_perm[ a[i] ] = TRUE;
//
//	*/
//
//	for (i = 1; i < k; i++) in_perm[a[i]] = TRUE; // we are finding candidates for a_k, a_k+1, ... a_n
//	// when k == 1, all candidates are valid because we haven't selected any yet
//	// when k == 2, all candidates except a_1 are valid
//	// when k == n, all candidates except a_1 .. a_n-1 are valid
//	*ncandidates = 0;
//	for (i = 1; i <= n; i++) {
//		if (in_perm[i] == FALSE) {
//			c[*ncandidates] = i;
//			*ncandidates = *ncandidates + 1;
//		}
//	}
//}

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