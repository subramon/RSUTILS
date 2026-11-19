// gcc is_valid_json.c -lcjson
// ./a.out
#include <stdio.h>
#include <stdbool.h>
#include "cjson/cJSON.h"
#include "is_valid_json.h"

/**
 * Validates whether a given string is valid JSON.
 * Returns true if valid, false otherwise.
 */
bool 
is_valid_json(const char *str) 
{
  if (str == NULL) {
    return false;
  }

  const char *error_ptr = NULL;
  // Parse the string. 
  // The third argument (1 / true) requires the string to be fully null-terminated 
  // after the JSON element, preventing trailing garbage characters.
  cJSON *root = cJSON_ParseWithOpts(str, &error_ptr, 1);

  if (root == NULL) {
    return false; // Parsing failed (invalid syntax)
  }

  // Clean up allocated memory and return success
  cJSON_Delete(root);
  return true;
}
#undef UNIT_TEST
#ifdef UNIT_TEST

int main(void) {
    // Test cases
    const char *test1 = "{\"name\": \"Alice\", \"age\": 30, \"active\": true}";
    const char *test2 = "[1, 2, 3, \"four\"]";
    const char *test3 = "{\"name\": \"Bob\",}"; // Invalid: trailing comma
    const char *test4 = "Just a plain string";  // Invalid: not JSON
    const char *test5 = "{\"a\": 1} extra";     // Invalid: trailing characters

    printf("Test 1: %s\n", is_valid_json(test1) ? "Valid JSON" : "Invalid JSON");
    printf("Test 2: %s\n", is_valid_json(test2) ? "Valid JSON" : "Invalid JSON");
    printf("Test 3: %s\n", is_valid_json(test3) ? "Valid JSON" : "Invalid JSON");
    printf("Test 4: %s\n", is_valid_json(test4) ? "Valid JSON" : "Invalid JSON");
    printf("Test 5: %s\n", is_valid_json(test5) ? "Valid JSON" : "Invalid JSON");

    return 0;
}
#endif
