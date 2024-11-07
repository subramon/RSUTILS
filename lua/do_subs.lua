-- local plfile = require 'pl.file'
function write_all(file, str)
  local fp = io.open(file, "w")
  fp:write(str)
  fp:close()
end
function read_all(file)
    local f = assert(io.open(file, "rb"))
    local content = f:read("*all")
    f:close()
    return content
end
-- local plpath = require 'pl.path'
function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end
local function do_subs(tmpl_file, out_file, replacements)
  assert(file_exists, "File not found " .. tmpl_file)
  assert(type(replacements) == "table")
  local tmpl = read_all(tmpl_file)
  assert(#tmpl > 0)

  local out = tmpl
  for k, v in pairs(replacements) do 
    out = string.gsub(out, k, v)
  end
  write_all(out_file, out)
  assert(plpath.isfile(out_file), "file not created " .. out_file)
  return true
end
return do_subs
