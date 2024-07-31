-- like stringify but uses ffi.new
local ffi = require 'ffi'
local function lstringify(str)
  assert(type(str) == "string")
  assert(#str >= 0) -- note that empty strings are allowed
  local len = #str + 1
  local cstr = assert(ffi.new("char[?]", len))
  ffi.copy(cstr, str, len-1)
  return ffi.cast("char *", cstr)
end
return lstringify
