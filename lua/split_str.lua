-- Assumptions 
-- (1) delimiter is a single character 
-- (2) delimiter is not a special character
local is_in = require 'RSUTILS/lua/is_in'

local function split_str(str, delim)
  assert(type(str) == "string")
  assert(type(delim) == "string")
  assert(#delim == 1) --- 
  assert(not is_in(delim, { 
    ".", 
    "%", 
    "*", 
    "+", 
    "-", 
    "?", 
    "^", 
    "$", 
    "[", 
    "]", 
    "(", 
    ")", 
  }))
  local T = {}
  while true do 
    local n1, n2 = string.find(str, delim)
    if ( not n1 ) then
      -- print("Extracted", #T+1, str)
      T[#T+1] = str
      break
    end
    local extracted = string.sub(str, 1, n1-#delim)
    -- print("Extracted", #T+1, extracted)
    local remaining = string.sub(str, n1+#delim, -1)
    -- print("Remaining", #T+1, remaining)
    str = remaining
    T[#T+1] = extracted
  end 
  return T
end
return split_str
--[[
-- quick and dirty test
local str = "abc:defg:hijkl"
--======================================
local D = split_str(str, ",")
assert(type(D) == "table")
assert(#D == 1)
assert(D[1] == str)
--======================================
local D = split_str(str, ":")
for k, v in ipairs(D) do print(k, v) end
assert(type(D) == "table")
assert(#D == 3)
assert(D[1] == "abc")
assert(D[2] == "defg")
assert(D[3] == "hijkl")
--======================================
--]]
