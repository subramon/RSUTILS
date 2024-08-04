local cutils = require 'libcutils'
-- Checks whether cdef'd file is available in cache_root directory
-- returns the name of the file and whether it exists or not 
local function is_cdef_cached(
  infile,
  src_root,
  cache_root
  )
  assert(type(infile) == "string")
  assert(type(src_root) == "string"); assert(cutils.isdir(src_root))
  assert(type(cache_root) == "string"); assert(cutils.isdir(cache_root))
  -- Determine the input file 
  -- If we do not have fully qualified path for infile, make it 
  if ( string.find(infile, "/") ~= 1 ) then
    assert(type(src_root) == "string")
    infile = cutils.realpath(src_root .. "/" .. infile)
  end
  assert(cutils.isfile(infile), "File not found: " .. infile)

  -- START: make name of cdef file 
  -- We strip src_root from infile 
  local stripped
  local n1, n2 = string.find(infile, src_root)
  if ( n1 ) then 
    assert(type(n1) == "number")
    assert(n1 == 1)
    stripped = string.sub(infile, n2)
  else
    stripped = infile
  end
  stripped = string.gsub(stripped, "/", "_")

  local cdef_file = cache_root .. stripped
  -- STOP : make name of cdef file 
  local is_file =  cutils.isfile(cdef_file) 
  return cdef_file, is_file 
end
return is_cdef_cached
