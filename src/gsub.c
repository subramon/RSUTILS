#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "q_macros.h"

// Function to replace all occurrences of b in a with c
int
gsub(
    const char* a,
    const char* b,
    const char* c,
    char **ptr_result
    )
{
  int status = 0;
  *ptr_result = NULL;
  if (!a || !b || !c) { go_BYE(-1); }
  size_t len_a = strlen(a);
  size_t len_b = strlen(b);
  size_t len_c = strlen(c);

  if ( len_b == 0 ) {
    *ptr_result = strdup(a); // avoid infinite loop
    goto BYE;
  }

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
  if (!result) { go_BYE(-1); }

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

  *ptr_result = result;
BYE:
  return status;
}

#undef TEST
#ifdef TEST
// Example usage
int main(
    void
    )
{
  int status =  0;
  const char* a = "the cat sat on the catmat";
  const char* b = "cat";
  const char* c = "dog";
  char *replaced = NULL;

  status = gsub(a, b, c, &replaced);
  if (replaced) {
    printf("Result: %s\n", replaced);
  }
BYE:
  free_if_non_null(replaced);
  return status;
}

#endif
