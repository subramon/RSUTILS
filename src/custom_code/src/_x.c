    json_t *x = json_object_get(root, "intercept");
    if ( x != NULL ) { 
      if ( json_is_real(x) ) { 
        Y[i].intercept = json_real_value(x); 
      }
      else if ( json_is_integer(x) ) { 
        Y[i].intercept = json_integer_value(x); 
      }
      else {
        go_BYE(-1);
      }
    }
  }
