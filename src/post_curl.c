#include <curl/curl.h>
#include "q_macros.h"
#include "post_curl.h"

int
post_curl(
    CURL *ch,
    const char *url,
    const char *const post_data,
    long *ptr_http_code
    )
{
  int status = 0; 
  CURLcode curl_res;
  
  curl_easy_setopt(ch, CURLOPT_URL, url);
  if ( ( post_data != NULL ) && ( *post_data != '\0' ) ) {
    curl_easy_setopt(ch, CURLOPT_POSTFIELDS, post_data);
  }
  // 3. (Optional) Force traditional form POST headers if needed,
  // though CURLOPT_POSTFIELDS adds this header automatically.
  // Users on Stack Overflow note that for custom/raw POST types (like JSON),
  // adding CURLOPT_POST is highly recommended.
  curl_easy_setopt(ch, CURLOPT_POST, 1L);

  // 4. Set up the callbacks to capture the server response text
  // Commented out because assumption is that this has already been done
  // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  // curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

  curl_res = curl_easy_perform(ch);
  if ( curl_res < 0 ) { go_BYE(-1); }
  curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, ptr_http_code);
BYE:
  return status;
}
