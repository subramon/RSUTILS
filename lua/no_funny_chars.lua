local is_in = require 'RSUTILS/lua/is_in'

local X = { 
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", 
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", 
  "_", ".", "-", }

local ok_chars = {}
for k, v in ipairs(X) do
  ok_chars[v] = true
end 

local function no_funny_chars(str, addnl)
  assert(type(str) == "string")
  for i = 1, #str do
     local c = string.sub(str, i, i)
     if ( ok_chars[c] or 
       ( ( type(addnl) == "table") and is_in(c, addnl) ) ) then
        -- all is well
      else
       return false
     end
   end
   return true
end
return no_funny_chars
