local ffi = require 'ffi'
pcall(ffi.cdef, "extern void *malloc(size_t size);")
local function multiple8(x)
  assert(type(x) == "number")
  local y = math.floor(x/8.0)
  local z = math.ceil(x/8.0)
  local w = 0
  if ( y == z ) then w = x else w = z*8 end 
  return w + 8 
end
local function stringify(str, name) 
  assert(type(str) == "string")
  assert(#str >= 0) -- note that empty strings are allowed
  local len = #str + 1
  len = multiple8(len) -- I suspect that LuaJIT does some funny stuff
  -- I get suspicious errors from Valgrind
  -- didn't seem too expensive to afford this bit of paranoia
  local cstr = assert(ffi.C.malloc(len))
  ffi.fill(cstr, len)
  ffi.copy(cstr, str, len-1)
  --[[ -- for debugging
  if ( name ) then 
    assert(type(name) == "string")
    print("Allocated " .. len .. " for " .. name)
  end
  --]]
  return ffi.cast("char *", cstr)
end
return stringify
