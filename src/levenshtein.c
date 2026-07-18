#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Required for abs() or custom min function
#include "levenshtein.h"

// Function to find the minimum of three integers
static int min_of_three(int a, int b, int c) {
    if (a <= b && a <= c) {
        return a;
    } else if (b <= a && b <= c) {
        return b;
    } else {
        return c;
    }
}

// Function to calculate the Levenshtein distance
int 
levenshtein_distance(
    const char *s1, 
    const char *s2
    ) 
{
  unsigned int s1len, s2len;
  s1len = strlen(s1);
  s2len = strlen(s2);

  // Create a 2D array (matrix) to store distances
  // The matrix size is (s2len+1) x (s1len+1)
  // Variable Length Arrays (VLAs) are used here, which are standard in C99.
  // For older C standards, dynamic memory allocation (malloc) would be needed.
  int matrix[s2len + 1][s1len + 1];

  // Initialize the first row and column
  // Distance from empty string to s1 prefix (deletion costs)
  for (unsigned int i = 0; i <= s1len; i++) {
    matrix[0][i] = i;
  }

  // Distance from empty string to s2 prefix (insertion costs)
  for (unsigned int j = 0; j <= s2len; j++) {
    matrix[j][0] = j;
  }

  // Fill the matrix using dynamic programming
  for (unsigned int i = 1; i <= s2len; i++) {
    for (unsigned int j = 1; j <= s1len; j++) {
      // If characters are the same, the cost is 0 (no operation needed)
      int cost = (s2[i - 1] == s1[j - 1]) ? 0 : 1;

      // Calculate the minimum cost:
      // 1. Deletion from s1 (matrix[i][j-1] + 1)
      // 2. Insertion into s1 (matrix[i-1][j] + 1)
      // 3. Substitution (matrix[i-1][j-1] + cost)
      matrix[i][j] = min_of_three(
          matrix[i - 1][j] + 1,      // Deletion
          matrix[i][j - 1] + 1,      // Insertion
          matrix[i - 1][j - 1] + cost // Substitution
          );
    }
  }

  // The bottom-right cell contains the final distance
  return matrix[s2len][s1len];
}

#undef TEST
#ifdef TEST
// Driver program to test the function
int main() {
    char s1[] = "kitten";
    char s2[] = "sitting";

    int distance = levenshtein_distance(s1, s2);

    printf("The Levenshtein distance between \"%s\" and \"%s\" is: %d\n", s1, s2, distance); // Expected output: 3

    return 0;
}

#endif // TEST
