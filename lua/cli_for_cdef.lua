local cutils = require 'libcutils'
local for_cdef = require 'RSUTILS/lua/for_cdef'

local infile = assert(arg[1])
local outfile 
if ( arg[2] ) then outfile = arg[2] end 
infile = cutils.realpath(infile)
-- print(infile)
local rsutils_src_root = assert(os.getenv("RSUTILS_SRC_ROOT"))
local x = for_cdef(infile, { rsutils_src_root .. "/inc/" })
if ( outfile ) then
  assert(type(outfile) == "string")
  assert(infile ~= outfile)
  assert(cutils.str_as_file(x, outfile))
else
  print(x)
end
