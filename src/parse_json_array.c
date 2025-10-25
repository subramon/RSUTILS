#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include "q_macros.h"
#include "parse_json_array.h"

int 
parse_json_array_I4(
      const char * const J,
      int32_t **ptr_X,
      uint32_t *ptr_nX
      )
{
  int status = 0; 
  int32_t *X = NULL; uint32_t nX = 0;
  // Parse the JSON array
  json_error_t error;
  json_t *json_array = json_loads(J, 0, &error);
  if ( json_array == NULL ) {
    fprintf(stderr, "Error parsing JSON: %s (line %d, column %d)\n",
        error.text, error.line, error.column);
    go_BYE(-1); 
  }

  if (!json_is_array(json_array)) {
    fprintf(stderr, "Input is not a valid JSON array.\n");
    go_BYE(-1); 
  }

  // Get number of elements
  nX = json_array_size(json_array);
  printf("Number of elements: %zu\n", nX);

  // Allocate C array
  X = malloc(nX * sizeof(int32_t));
  return_if_malloc_failed(X); 

  // Extract integers
  for (uint32_t i = 0; i < nX; i++) {
    json_t *value = json_array_get(json_array, i);
    if (!json_is_integer(value)) {
      fprintf(stderr, "Element %zu is not an integer.\n", i);
      go_BYE(-1);
    }
    X[i] = (int)json_integer_value(value);
  }
  *ptr_X = X;
  *ptr_nX = nX;
BYE:
  if ( json_array != NULL ) { json_decref(json_array); }
  return status;
}
#undef UNIT_TEST 
#ifdef UNIT_TEST 
int 
main() 
{
  int status = 0; 
  int32_t *X = NULL; uint32_t nX  = 0;  // to be created 
  const char *input = " [ 123, 4567, 89012 ] ";

  status = parse_json_array_I4(input, &X, &nX); cBYE(status);
  // Print the extracted array
  printf("Extracted integers: \n");
  for ( uint32_t  i = 0; i < nX; i++) {
    printf("%d: %d\n", i, X[i]);
  }
  printf("\n");

BYE:
  // Cleanup
  free_if_non_null(X);
  return status;
}
#endif
