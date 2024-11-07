-- Given an input file
-- 1) Extract portion(s) between
-- //START_FOR_CDEF and
-- //STOP_FOR_CDEF and
-- 2) Write this to a temp file
-- 2) If incs provided in a table, create an appropriate string e.g., -I../inc/
-- 3) Run the temp file through the C pre-processor with a -I string if needed
-- 3)  Remove any line that starts with # This gets rid of
--    #ifdef
--    #ifndef
--    #endif
--    #define
-- 4) Return 3 things
--  4a) a string with the above
--  4b) whether you found the cdef str in the cache 
--  4c) name of file in the cache where the cdef str would have been stored

local exec   = require 'RSUTILS/lua/exec_and_capture_stdout'
local c_exec = require 'RSUTILS/lua/c_exec'
local is_cdef_cached = require 'RSUTILS/lua/is_cdef_cached'
local cutils = require 'libcutils'

-- Creates file with name cdef_file which contains
-- the function declaration for the function(s) specified in infile
-- which are enclosed between START_FOR_CDEF and STOP_FOR_CDEF
local function for_cdef(
  infile,
  incs,
  cache_root,
  src_root
  )
  -- Determine the input file 
  assert(type(infile) == "string")
  if ( string.sub(infile, 1, 1) ~= "/") then 
    -- we do not have fully qualified path
    assert(type(src_root) == "string")
    infile = src_root .. "/" .. infile
    assert(cutils.isdir(src_root), "Not a directory " .. src_root)
  end
  assert(cutils.isfile(infile), "File not found: " .. infile)
  --=======================================================
  local cdef_file
  if ( cache_root ) then 
    local cdef_file, is_cdef_file = is_cdef_cached(infile, src_root, cache_root)
    if ( is_cdef_file ) then 
      return cutils.file_as_str(cdef_file), true, cdef_file
    end
  end
  --=======================================================
  -- Control comes here means that we didn't get lucky and find
  -- the cdef_str in the cache. So, we have to create it 
  --============================================
  -- Determine the incs i.e., -I....
  local cmd
  if ( incs ) then
    assert(type(incs) == "table")
    local str_incs = {}
    for k, v in ipairs(incs) do
      local incdir 
      if ( string.find(v, "/") ~= 1 ) then
        -- we do not have fully qualified path
        incdir = src_root .. "/" .. v
      else
        incdir = v
      end 
      assert(cutils.isdir(incdir), "Missing directory " .. incdir)
      str_incs[k] = "-I" .. incdir
    end
    incs = table.concat(str_incs, " ")
  else
    incs = ""
  end
  --===================
  local X = {}
  local fp = assert(io.open(infile))
  local is_write = true
  local cnt = 0
  for line in fp:lines() do
    if ( string.find(line, "START_FOR_CDEF", 1) ) then
      assert(is_write == true)
      is_write = false
      cnt = cnt + 1
    end
    if ( not is_write ) then
      X[#X + 1] = line
    end
    if ( string.find(line, "STOP_FOR_CDEF", 1) ) then
      assert(is_write == false)
      is_write = true
      cnt = cnt - 1
    end
  end
  assert(cnt == 0, "Mismatch between start/stop of cdef markers")
  fp:close()
  local tmpfile = os.tmpname()
  fp = io.open(tmpfile, "w")
  fp:write(table.concat(X, "\n"))
  fp:close()
  --===================
  cmd = string.format(" cpp %s %s |grep -v '^#'",
      tmpfile, incs)
  -- OLD local  rslt = assert(exec(cmd), "Failed " .. cmd)
  local  rslt = assert(c_exec(cmd), "Failed " .. cmd)
  os.remove(tmpfile)

  -- check that you do not get back empty string
  local chk = string.gsub(rslt, "%s", "")
  assert(#chk > 0, "infile = " .. infile)
  --==============
  return rslt, false, cdef_file
end
return for_cdef
--[[ qd test 
x = for_cdef("CMEM/inc/cmem_struct.h", 
  { "/home/subramon/RSUTILS/inc/" }, false, "/home/subramon/" )
print(x)
--]]
