//START_INCLUDES
#include <omp.h>
//STOP_INCLUDES
#include "q_omp_get_num_procs.h"
//START_FUNC_DECL
int 
q_omp_get_num_procs(
    void
    )
//STOP_FUNC_DECL
{
  return omp_get_num_procs();
}
