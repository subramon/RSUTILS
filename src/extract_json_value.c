#include <stdio.h>
#include <jansson.h>
#include "q_incs.h"
#include "extract_json_value.h"

int
extract_json_value(
    const char * const in_str,
    const char * const key,
    char *out_str, /* [len] */
    size_t maxlen
    )
// STOP FUNC DECL
{
  int status = 0;
  json_t *jroot = NULL; json_error_t jerror; 
  json_t *jval = NULL;

  jroot = json_loads(in_str, 0, &jerror);
  if ( jroot == NULL ) { go_BYE(-1); } 
  jval = json_object_get(jroot, key);
  if ( jval == NULL ) { go_BYE(-1); }
  if ( !json_is_string(jval)) { go_BYE(-1); }
  const char *string_value = json_string_value(jval);
  size_t len = strlen(string_value);
  if ( len > maxlen ) { go_BYE(-1); }
  strcpy(out_str, string_value);



BYE:
  if ( jval != NULL ) { json_decref(jval); }
  if ( jroot != NULL ) { json_decref(jroot); }
  return status;
}
