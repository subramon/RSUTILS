local ffi = require 'ffi'
ffi.cdef([[
extern       void *malloc(size_t size);
extern       void free(void *ptr);
extern       void *calloc(size_t nmemb, size_t size);
extern       void *realloc(void *ptr, size_t size);
extern       void *reallocarray(void *ptr, size_t nmemb, size_t size);
]]
)
local read_csv_spec = require 'RSUTILS/lua/read_csv_spec'
-- start sample data 
local lS = {}
lS.break_cols = { "abc", "def", }
lS.is_hdr = true
lS.fld_sep = "comma"
lS.fld_delim = "dquote"
lS.rec_sep = "eoln"

local tests = {}
tests.t1 = function()
  local cS = ffi.C.malloc(ffi.sizeof("csv_spec_t"))
  ffi.fill(cS, ffi.sizeof("csv_spec_t"), 0)
  cS = ffi.cast("csv_spec_t *", cS)
  assert(read_csv_spec(lS, cS))
  print("nB = ", cS[0].nB)
  print("break1 = ", ffi.string(cS[0].break_cols[0]))
  print("break2 = ", ffi.string(cS[0].break_cols[1]))
  ffi.C.free(cS[0].break_cols[0])
  ffi.C.free(cS[0].break_cols[1])
  ffi.C.free(cS[0].break_cols)
  ffi.C.free(cS[0].str_fld_sep)
  ffi.C.free(cS[0].str_rec_sep)
  ffi.C.free(cS[0].str_fld_delim)
  ffi.C.free(cS)
  print("Success on test t1 ");
end
-- return tests
tests.t1()
