#include <stdio.h>
#include <time.h>
#include "get_time_as_timestamp.h"

/**
 * Retrieves the current local time in "YYYY-MM-DD HH:mm:SS" format.
 * 
 * @param buffer Pointer to the character array to store the formatted string.
 * @param size   The maximum size of the buffer.
 */
void 
get_time_as_timestamp(
    char *buffer, 
    size_t size
    ) 
{
  time_t raw_time;
  struct tm *time_info;

  // Get current calendar time
  time(&raw_time);

  // Convert to local time structure
  time_info = localtime(&raw_time);

  // Format the time into the provided buffer
  strftime(buffer, size, "%Y-%m-%d %H:%M:%S", time_info);
}
