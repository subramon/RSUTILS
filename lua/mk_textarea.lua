local cutils = require 'libcutils'
local is_in = require 'RSUTILS/lua/is_in'
local ok_chars = { 
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", 
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", 
  "_", ".", "-", }

local function no_funny_chars(str, addnl)
  assert(type(str) == "string")
  for i = 1, #str do
     local c = string.sub(str, i, i)
     if ( ( is_in(c, ok_chars) ) or 
        ( ( type(addnl) == "table") and is_in(c, addnl) ) ) then
        -- all is well
      else
       return false
     end
   end
   return true
end
local function  mk_textarea(disp_str, aux)
  assert(type(disp_str) == "string")
  assert(type(aux) == "table")
  local X = {}
  X[#X+1] = "<textarea "
  --=== handle rows 
  if ( aux.rows ) then 
    assert(type(aux.rows) == "number")
    assert(aux.rows >= 1)
    X[#X+1] = string.format('rows = "%d" ', aux.rows)
  end
  --=== handle cols
  if ( aux.cols ) then 
    assert(type(aux.cols) == "number")
    assert(aux.cols >= 1)
    X[#X+1] = string.format('cols = "%d" ', aux.cols)
  end
  --=== handle maxlength 
  if ( aux.maxlength ) then 
    assert(type(aux.maxlength) == "number")
    assert(aux.maxlength >= 1)
    X[#X+1] = string.format('maxlength = "%d" ', aux.maxlength)
  end
  --=== handle minlength 
  if ( aux.minlength ) then 
    assert(type(aux.minlength) == "number")
    assert(aux.minlength >= 1)
    X[#X+1] = string.format('minlength = "%d" ', aux.minlength)
  end
  --=====
  if ( ( aux.maxlength ) and ( aux.minlength ) ) then 
    assert(aux.maxlength >= aux.minlength)
  end
  --=== handle wrap 
  local wrap 
  if ( aux.wrap ) then 
    assert(type(aux.wrap) == "string")
    assert(is_in(aux.wrap, { "soft", "hard", "off", }))
  else
    aux.wrap = "soft" -- default 
  end
  X[#X+1] = string.format('wrap = "%s" ', aux.wrap)
  --=== handle id 
  local id 
  if ( aux.id ) then 
    assert(type(aux.id) == "string")
    assert(no_funny_chars(aux.id))
    X[#X+1] = string.format('id = "%s" ', aux.id)
  end 
  --=== handle name
  local name 
  if ( aux.name ) then 
    assert(type(aux.name) == "string")
    assert(no_funny_chars(aux.name))
    X[#X+1] = string.format('name = "%s" ', aux.name)
  end 
  --=== handle placeholder 
  local placeholder 
  if ( aux.placeholder ) then 
    assert(type(aux.placeholder) == "string")
    assert(no_funny_chars(aux.name, { " ", }))
    X[#X+1] = string.format('placeholder = "%s" ', aux.placeholder)
  end 
  if ( aux.readonly ) then 
    assert(type(aux.readonly) == "boolean")
    X[#X+1] = " readonly "
  end 
  X[#X+1] = " > "
  X[#X+1] = cutils.html_escape(disp_str)
  X[#X+1] = "</textarea>"
  return table.concat(X, "")
end
--[[ UNIT TEST 
return  mk_textarea
--]]
local aux = {}
local disp_str = "This is a text area "
aux.rows = 10
aux.cols = 20
aux.minlength = 30
aux.maxlength = 40
aux.readonly = true 
aux.id = "id_xx"
aux.name = "name_xx"
aux.wrap = "soft"
aux.placeholder = "placeholder xx"

print(mk_textarea(disp_str, aux))


