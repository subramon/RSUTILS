local ffi = require 'ffi'
pcall(ffi.cdef, "extern void *malloc(size_t size);")
local function tbl_to_C_2d(J)
  assert(type(J) == "table")
  assert(#J > 0)
  local sz = #J * ffi.sizeof("char *")
  local T = ffi.C.malloc(sz)
  T = ffi.cast("char **", T)
  local n = 0
  for i, sval in ipairs(J) do 
    assert(type(sval) == "string")
    local len = (#sval+1) * ffi.sizeof("char")
    T[i-1] = ffi.C.malloc(len)
    ffi.fill(T[i-1], len, 0)
    if ( #sval > 0 ) then 
      ffi.copy(T[i-1], sval)
    end
    n = n + 1
  end
  assert(n ~= 0)
  T = ffi.cast("char ** ", T)
  return T, n
end
return tbl_to_C_2d
