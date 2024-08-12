local ffi = require 'ffi'
pcall(ffi.cdef, "extern void *malloc(size_t size);")
local function tbl_to_C_2d(J)
  assert(type(J) == "table")
  assert(#J > 0)
  local T = ffi.C.malloc(#J * ffi.sizeof("char *"))
  T = ffi.cast("char **", T)
  local n = 0
  for i, sval in ipairs(J) do 
    assert(type(sval) == "string")
    T[i-1] = ffi.C.malloc((#sval+1) * ffi.sizeof("char"))
    -- needed? ffi.C.memset(T[i(#sval+1) * sizeof("char"))
    ffi.copy(T[i-1], sval)
    n = n + 1
  end
  assert(n ~= 0)
  T = ffi.cast("char ** ", T)
  return T, n
end
return tbl_to_C_2d
