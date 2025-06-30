#include "q_incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "cat_to_buf.h"
#include "csv_meta_t.h"
#include "csv_meta_aux.h"

void
csv_meta_free(
    csv_meta_t  *ptr_M
    )
{
  free_2d_array(&(ptr_M->col_names),  ptr_M->nC);
  free_if_non_null(ptr_M->widths);
  free_2d_array(&(ptr_M->formats),  ptr_M->nC);
  free_if_non_null(ptr_M->qtypes);
  free_if_non_null(ptr_M->has_nulls);
  free_if_non_null(ptr_M->is_load);
}

char *
csv_meta_to_json(
    csv_meta_t  *ptr_M
    )
{
  int status = 0;
  char *buf = NULL; uint32_t bufsz = 1024; uint32_t buflen = 0;
  buf = malloc(bufsz); bzero(buf, bufsz); 
  const char *preamble = 
    "  <!DOCTYPE html> "
    "<html> "
    "<body> "
    "  <center> "
    "<style> "
    "    table, th, td { "
    "  border: 1px solid black; "
    "} "
    "</style> "
    " <body> "
    " <table> ";

  status = cat_to_buf(&buf, &bufsz,  &buflen, preamble, 0); cBYE(status);
  // table header 
  char hdr[1024]; 
  strcpy(hdr, 
      "<thead><tr> "
      "<td> <b> Column </b> </td> "
      "<td> <b> Width </b> </td> "
      "<td> <b> Format </b> </td> "
      "<td> <b> qtype </b> </td> "
      "<td> <b> has_nulls </b> </td> "
      "<td> <b> is_load </b> </td> "
      "</tr></thead> ");
  status = cat_to_buf(&buf, &bufsz,  &buflen, hdr, 0); cBYE(status);
  for ( uint32_t i = 0; i < ptr_M->nC; i++ ) { 
    status = cat_to_buf(&buf, &bufsz,  &buflen, "<tr>", 0); cBYE(status);

    status = cat_to_buf(&buf, &bufsz,  &buflen, "<td>", 0); cBYE(status);
    status = cat_to_buf(&buf, &bufsz,  &buflen, ptr_M->col_names[i], 0); 
    status = cat_to_buf(&buf, &bufsz,  &buflen, "</td>", 0); cBYE(status);
    //-----------------------------------------------------------
    char tmp[64]; memset(tmp, 0, 64); 
    status = cat_to_buf(&buf, &bufsz,  &buflen, "<td>", 0); cBYE(status);
    sprintf(tmp, "%u", ptr_M->widths[i]); 
    status = cat_to_buf(&buf, &bufsz,  &buflen, tmp, 0); cBYE(status);
    status = cat_to_buf(&buf, &bufsz,  &buflen, "</td>", 0); cBYE(status);
    //-----------------------------------------------------------
    status = cat_to_buf(&buf, &bufsz,  &buflen, "<td>", 0); cBYE(status);
    if ( ptr_M->formats[i] == NULL ) { 
      strcpy(tmp, " -- ");
    }
    else {
      strcpy(tmp, ptr_M->formats[i]);
    }
    status = cat_to_buf(&buf, &bufsz,  &buflen, tmp, 0); cBYE(status);
    status = cat_to_buf(&buf, &bufsz,  &buflen, "</td>", 0); cBYE(status);
    //-----------------------------------------------------------
    status = cat_to_buf(&buf, &bufsz,  &buflen, "<td>", 0); cBYE(status);
    status = cat_to_buf(&buf, &bufsz,  &buflen, 
        get_str_qtype(ptr_M->qtypes[i]), 0);
    status = cat_to_buf(&buf, &bufsz,  &buflen, "</td>", 0); cBYE(status);
    //-----------------------------------------------------------
    status = cat_to_buf(&buf, &bufsz,  &buflen, "<td>", 0); cBYE(status);
    if ( ptr_M->has_nulls[i] ) { 
      strcpy(tmp, "true");
    }
    else {
      strcpy(tmp, "false"); 
    }
    status = cat_to_buf(&buf, &bufsz,  &buflen, tmp, 0); cBYE(status);
    status = cat_to_buf(&buf, &bufsz,  &buflen, "</td>", 0); cBYE(status);
    //-----------------------------------------------------------
    status = cat_to_buf(&buf, &bufsz,  &buflen, "<td>", 0); cBYE(status);
    if ( ptr_M->is_load[i] ) { 
      strcpy(tmp, "true");
    }
    else {
      strcpy(tmp, "false"); 
    }
    status = cat_to_buf(&buf, &bufsz,  &buflen, tmp, 0); cBYE(status);
    status = cat_to_buf(&buf, &bufsz,  &buflen, "</td>", 0); cBYE(status);
    //-----------------------------------------------------------
    status = cat_to_buf(&buf, &bufsz,  &buflen, "</tr>", 0); cBYE(status);

  }
  const char *postamble = "</table> </body></html>";
  status = cat_to_buf(&buf, &bufsz,  &buflen, postamble, 0); cBYE(status);

BYE:
  if ( status != 0 ) { return NULL; } else { return buf; }
}

#undef TEST
#ifdef TEST
int
main()
{
  int status = 0;
  csv_meta_t M; memset(&M, 0,  sizeof(M));
  M.nC = 10;
  M.col_names = malloc(M.nC * sizeof(char *));
  M.widths = malloc(M.nC * sizeof(uint32_t));
  M.formats = malloc(M.nC * sizeof(char *));
  M.qtypes = malloc(M.nC * sizeof(qtype_t));
  M.has_nulls = malloc(M.nC * sizeof(bool));
  M.is_load = malloc(M.nC * sizeof(bool));
  char buf[64];
  for ( int i = 0; i < M.nC; i++ ) { 
    sprintf(buf, "name_%d \n", i+1); 
    M.col_names[i] = strdup(buf); 

    M.qtypes[i] = i+1; // dangerous but okay for testing 

    sprintf(buf, "format_%d \n", i+1); 
    M.formats[i] = strdup(buf); 

    M.widths[i] = get_width_c_qtype(M.qtypes[i]); 

    M.has_nulls[i] = i % 2 ; // dangerous but okay for testing 
    M.is_load[i] = (i+1) % 2 ; // dangerous but okay for testing 
  }
  char *opbuf = csv_meta_as_json(&M);
  printf("%s\n", opbuf);

  csv_meta_free(&M); 
BYE:
  free_if_non_null(opbuf); 
  return status;
}


#endif
