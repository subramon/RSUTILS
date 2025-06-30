#include "q_incs.h"
#include <yaml.h>
bool
is_valid_yaml(
    const char * const str,
    size_t len
    )
{
  int status = 0;
  yaml_parser_t parser;
  if (!yaml_parser_initialize(&parser)) { go_BYE(-1); }
  if ( ( str == NULL ) || ( len == 0 ) ) { go_BYE(-1); }
  yaml_parser_set_input_string(&parser, str, len);
  yaml_event_t event;
  int done = 0; bool error = false;
  while (!done) {
    if (!yaml_parser_parse(&parser, &event)) {
      error = true; 
      done = 1; // Stop parsing on error
    } 
    else {
      // Process the event (optional, for validation purposes you might just discard it)
      if (event.type == YAML_STREAM_END_EVENT) { done = 1; }
      yaml_event_delete(&event); // Free the event memory
    }
  }
  yaml_parser_delete(&parser);
BYE:
  if ( status != 0 ) { return false; } else { return true; }
}
