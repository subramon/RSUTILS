#include <stddef.h>
#include "swap_bytes.h"
void 
swap_bytes(
    void *value, 
    size_t size
    ) 
{
  unsigned char *ptr = value;
  for (size_t i = 0; i < size / 2; i++) {
    unsigned char tmp = ptr[i];
    ptr[i] = ptr[size - 1 - i];
    ptr[size - 1 - i] = tmp;
  }
}
