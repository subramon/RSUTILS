#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Function to unescape common HTML entities in a string
void 
unescapeHtml( // works in situ
    char *str
    ) 
{
  int status = 0; 
  if (str == NULL) { return ; }

  char *read_ptr = str;
  char *write_ptr = str;

  while (*read_ptr != '\0') {
    if (*read_ptr == '&') {
      // Check for &amp;
      if (strncmp(read_ptr, "&amp;", 5) == 0) {
        *write_ptr++ = '&';
        read_ptr += 5;
        continue;
      }
      // Check for &lt;
      if (strncmp(read_ptr, "&lt;", 4) == 0) {
        *write_ptr++ = '<';
        read_ptr += 4;
        continue;
      }
      // Check for &gt;
      if (strncmp(read_ptr, "&gt;", 4) == 0) {
        *write_ptr++ = '>';
        read_ptr += 4;
        continue;
      }
      // Check for &quot;
      if (strncmp(read_ptr, "&quot;", 6) == 0) {
        *write_ptr++ = '\"';
        read_ptr += 6;
        continue;
      }
      // Check for &#39; (single quote)
      if (strncmp(read_ptr, "&#39;", 5) == 0) {
        *write_ptr++ = '\'';
        read_ptr += 5;
        continue;
      }
    }
    // If not an entity, copy the character as is
    *write_ptr++ = *read_ptr++;
  }
  *write_ptr = '\0'; // Null-terminate the unescaped string
}

#undef UNIT_TEST
#ifdef UNIT_TEST
int main() 
{
  char html_string1[] = "This is &lt;b&gt;bold&lt;/b&gt; text with an &amp; symbol and a &quot;quote&quot;.";
  char html_string2[] = "Another string with &#39;single quotes&#39;.";
  char html_string3[] = "No entities here.";

  printf("Original 1: %s\n", html_string1);
  unescapeHtml(html_string1);
  printf("Unescaped 1: %s\n\n", html_string1);

  printf("Original 2: %s\n", html_string2);
  unescapeHtml(html_string2);
  printf("Unescaped 2: %s\n\n", html_string2);

  printf("Original 3: %s\n", html_string3);
  unescapeHtml(html_string3);
  printf("Unescaped 3: %s\n\n", html_string3);

  return 0;
}
#endif
