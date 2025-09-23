#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "q_macros.h"

// Function to replace all occurrences of b in a with c
char* 
gsub(
    const char* a, 
    const char* b, 
    const char* c
    ) 
{
  int status = 0; 
  if (!a || !b || !c) { go_BYE(-1); } 
  size_t len_a = strlen(a);
  size_t len_b = strlen(b);
  size_t len_c = strlen(c);

  if (len_b == 0) return strdup(a); // avoid infinite loop

  // First pass: count occurrences of b in a
  size_t count = 0;
  const char* pos = a;
  while ((pos = strstr(pos, b)) != NULL) {
    count++;
    pos += len_b;
  }

  // Allocate memory for new string
  size_t new_len = len_a + count * (len_c - len_b) + 1;
  char* result = malloc(new_len);
  if (!result) return NULL;

  // Second pass: build the new string
  const char* src = a;
  char* dst = result;
  while ((pos = strstr(src, b)) != NULL) {
    size_t prefix_len = pos - src;
    memcpy(dst, src, prefix_len);
    dst += prefix_len;
    memcpy(dst, c, len_c);
    dst += len_c;
    src = pos + len_b;
  }
  // Copy the remainder
  strcpy(dst, src);

BYE:
  if ( status == 0 ) { return result; } else { return NULL; } 
}

#undef TEST
#ifdef TEST
// Example usage
int main() {
    const char* a = "the cat sat on the catmat";
    const char* b = "cat";
    const char* c = "dog";

    char* replaced = gsub(a, b, c);
    if (replaced) {
        printf("Result: %s\n", replaced);
        free(replaced);
    }

    return 0;
}

#endif
