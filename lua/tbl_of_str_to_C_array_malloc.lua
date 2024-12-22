local ffi = require 'ffi'
local function tbl_of_str_to_C_array(J)
  assert(type(J) == "table")
  local T = ffi.C.malloc(#J * ffi.sizeof("char **"))
  T = ffi.cast("char **", T)
  ffi.fill(T, #J * ffi.sizeof("char **"), 0)
  local n = 0
  for i, sval in ipairs(J) do 
    assert(type(sval) == "string")
    T[i-1] = stringify(sval)
    n = n + 1
  end
  assert(n ~= 0)
  return T, n
end
return tbl_of_str_to_C_array
