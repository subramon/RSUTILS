local cutils  = require 'libcutils'

local section = { 
  c = 'definition', 
  h = 'declaration',
  ispc = 'definition', 
  }

-- The src_root is needed if a fully qualified path is not provided
-- either for the template file (tmpl) or the output directory (opdir)
local function do_replacements(subs, lang, src_root)
  assert ( ( lang == "C" ) or ( lang == "ISPC" ) ) 
  local tmpl
  if ( lang == "C" ) then tmpl = assert(subs.tmpl) end 
  if ( lang == "ISPC" ) then tmpl = assert(subs.tmpl_ispc) end 
  if ( string.sub(tmpl, 1, 1) ~= "/") then -- TODO P4: What if no '/'? 
    assert(type(src_root) == "string")
    tmpl = src_root .. tmpl
  end
  local T
  assert(cutils.isfile(tmpl), "File not found " .. tmpl)
  T = assert(dofile(tmpl))
  for k, v in pairs(subs) do
     T[k] = v
  end
  return T
end

local _dotfile = function(subs, opdir, lang, ext, src_root)
  assert(type(opdir) == "string")
  assert(#opdir > 0)
  local func_name = subs.fn
  local basic_fname = opdir .. "/" .. func_name .. "." .. ext
  if ( string.sub(opdir, 1, 1) ~= "/") then -- TODO P4: What if no '/' ?
    assert(type(src_root) == "string")
    opdir = src_root .. opdir
  end
  if ( not cutils.isdir(opdir) ) then
    assert(cutils.makepath(opdir), "Unable to create dir " .. opdir)
  end
  assert(cutils.isdir(opdir))
  local T = do_replacements(subs, lang, src_root)
  local dotfile = T(section[ext])
  local fname = opdir .. "/" .. func_name .. "." .. ext
  local f = assert(io.open(fname, "w"))
  assert(f, "Unable to open file " .. fname)
  f:write(dotfile)
  f:close()
  -- Note that we return basic_fname, not fname for consistency reasons
  return basic_fname, fname
end

local fns = {}

fns.dotc = function (subs, opdir, src_root)
  return _dotfile(subs, opdir, "C", 'c', src_root)
end

fns.doth = function (subs, opdir, src_root)
  return _dotfile(subs, opdir, "C", 'h', src_root)
end

fns.ispc = function (subs, srcdir, incdir, src_root)
  -- this will return .ispc file and .h file 
  -- first create the .ispc file 
  local ispc_basic, ispc_full =  
    _dotfile(subs, srcdir, "ISPC", 'ispc', src_root)
  -- now create the .h file
  -- first, create its name (rather ugly repetition)
  local func_name = subs.fn_ispc
  local h_basic = incdir .. "/" .. func_name .. ".h"
  if ( string.sub(incdir, 1, 1) ~= "/") then -- TODO P4: What if no '/' ?
    assert(type(src_root) == "string")
    incdir = src_root .. incdir
  end
  if ( not cutils.isdir(incdir) ) then
    assert(cutils.makepath(incdir))
  end
  local h_full = incdir .. "/" .. func_name .. ".h" 
  -- now we use ispc to create the .h file 
  local cmd = "ispc " .. ispc_full .. " -h " .. h_full .. " 1>/dev/null 2>&1"
  if ( qcfg.use_ispc ) then 
    print("ispc making .h file", cmd)
    local status = os.execute(cmd)
    assert(status == 0)
  end
  return ispc_basic, h_basic

end
return fns
