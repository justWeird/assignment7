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

void backtrack(int a[], int k, int input, combWord allPerm[], int* permutation) {

   int c[MAXCANDIDATES];     /* candidates for next position  */
   int ncandidates;          /* next position candidate count */
   int i;                    /* counter                       */

   if (is_a_solution(a,k,input)) {
       process_solution(a, k, input, allPerm, permutation);
   } 
   else {
      k = k+1;
      construct_candidates_permutation(a,k,input,c,&ncandidates); 
	  // construct_candidates_subset(a,k,input,c,&ncandidates); 
      for (i=0; i<ncandidates; i++) {
         a[k] = c[i];
         //make_move(a,k,input);
         backtrack(a,k,input, allPerm, permutation);
         //unmake_move(a,k,input);
      }
   }
}



bool is_a_solution(int a[], int k, int n) {
   return (k == n);
}
 

void process_solution(int a[], int k, int input, combWord allPermutations[], int *permutationCount) {

   int i;                       /* counter */
   bool print_permuations;      /* flag ... set to true if you want the permutations listed to the terminal */

   print_permuations = false;

   if (print_permuations) {
	  for (i=1; i<=k; i++) {
         printf(" %d",a[i]);
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
   *permutationCount = *permutationCount + 1 ;


}
 


void construct_candidates_subset(int a[], int k, int n, int c[], int *ncandidates) {
   c[0] = TRUE;
   c[1] = FALSE;
   *ncandidates = 2;
}


void construct_candidates_permutation(int a[], int k, int n, int c[], int *ncandidates) {

   int i;                       /* counter */
   bool in_perm[NMAX];          /* who is in the permutation? */
   for (i=1; i<NMAX; i++) {     /* NB: NMAX must be the number of elements in the permutation + 1 to allow for counting from 1, rather than 0 */
	   in_perm[i] = FALSE;
   }

   /* original Skiena code is incorrect ... a[i(==0)] is not a valid element of the permutation since we start storing them at a[1]

      for (i=0; i<k; i++) in_perm[ a[i] ] = TRUE;

   */

   for (i=1; i<k; i++) in_perm[ a[i] ] = TRUE; // we are finding candidates for a_k, a_k+1, ... a_n
                                               // when k == 1, all candidates are valid because we haven't selected any yet
                                               // when k == 2, all candidates except a_1 are valid
                                               // when k == n, all candidates except a_1 .. a_n-1 are valid
   *ncandidates = 0;
   for (i=1; i<=n; i++) {
      if (in_perm[i] == FALSE) {
         c[ *ncandidates] = i;
         *ncandidates = *ncandidates + 1;
      }
   }
}

// Function to transform the original word based on a permutation
void transformWord(char* originalWord, int* permutation, char* transformedWord, int wordLength) {
    // Iterate through the permutation array
    for (int i = 0; i < wordLength; i++) {
        // The permutation array contains 1-based indices
        // So we subtract 1 to convert to 0-based indices for array access
        int originalIndex = permutation[i] - 1;

        // Copy the character from the original word to the transformed word
        // based on the permutation
        transformedWord[i] = originalWord[originalIndex];
    }

    transformedWord[wordLength] = '\0';

}





void prompt_and_exit(int status) {
   printf("Press any key to continue and close terminal\n");
   getchar();
   exit(status);
}