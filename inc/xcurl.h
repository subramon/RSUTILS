extern int
xcurl(
    const char * const url,
    const char * const method,
    const char * const post_data,
    const char ** curl_options, // [n_curl_opts]
    const char ** curl_params, // [n_curl_opts]
    int n_curl_opts, 
    long *ptr_http_code,
    char **ptr_str_response
    );
