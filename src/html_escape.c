#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "q_macros.h"
#include "html_escape.h"

// Function to escape special HTML characters in a string

int
html_escape(
    const char* input_string,
    char **ptr_output_string
    ) 
{
  int status = 0; 
  if (input_string == NULL) { go_BYE(-1); }

  // Estimate a safe buffer size (worst case: all chars need escaping, each becoming ~6 chars)
  size_t input_len = strlen(input_string);
  size_t output_buffer_size = input_len * 6 + 1; // +1 for null terminator
  char* output_string = (char*)malloc(output_buffer_size);

  if (output_string == NULL) {
    perror("Failed to allocate memory for escaped string");
    go_BYE(-1); 
  }

  size_t current_output_len = 0;

  for (size_t i = 0; i < input_len; ++i) {
    char c = input_string[i];
    const char* replacement = NULL;

    switch (c) {
      case '"':
        replacement = "&quot;";
        break;
      case '\'':
        replacement = "&#039;"; // Or &apos; for XML/XHTML/HTML5
        break;
      case '<':
        replacement = "&lt;";
        break;
      case '>':
        replacement = "&gt;";
        break;
      case '&':
        replacement = "&amp;";
        break;
      default:
        // No special handling needed for other characters
        break;
    }

    if (replacement != NULL) {
      // Check if there's enough space in the buffer
      size_t replacement_len = strlen(replacement);
      if (current_output_len + replacement_len >= output_buffer_size) {
        // Reallocate if necessary (though initial estimate should be generous)
        output_buffer_size = (current_output_len + replacement_len) * 2; // Double the size
        char* temp = (char*)realloc(output_string, output_buffer_size);
        if (temp == NULL) {
          perror("Failed to reallocate memory for escaped string");
          free(output_string);
          go_BYE(-1); 
        }
        output_string = temp;
      }
      strcpy(output_string + current_output_len, replacement);
      current_output_len += replacement_len;
    } else {
      if (current_output_len + 1 >= output_buffer_size) {
        output_buffer_size = (current_output_len + 1) * 2;
        char* temp = (char*)realloc(output_string, output_buffer_size);
        if (temp == NULL) {
          perror("Failed to reallocate memory for escaped string");
          free(output_string);
          go_BYE(-1); 
        }
        output_string = temp;
      }
      output_string[current_output_len++] = c;
    }
  }
  output_string[current_output_len] = '\0'; // Null-terminate the string

  *ptr_output_string = output_string;
BYE:
  return status;
}

#undef UNIT_TEST
#ifdef UNIT_TEST
int 
main() 
{
  int status = 0;
  const char* original_text = "This is a \"quoted\" string with 'single quotes', <tags> and & ampersands.";
  char* escaped_text = NULL;
  status = html_escape(original_text, &escaped_text); cBYE(status);

  if (escaped_text != NULL) {
    printf("Original: %s\n", original_text);
    printf("Escaped:  %s\n", escaped_text);

    // Example of how it would be used in HTML (conceptual)
    printf("\nHTML Usage (conceptual):\n");
    printf("<input type=\"text\" value=\"%s\">\n", escaped_text);
    printf("<textarea>%s</textarea>\n", escaped_text);

  }
BYE:
  free_if_non_null(escaped_text); // Free the dynamically allocated memory
  return status;
}
#endif
