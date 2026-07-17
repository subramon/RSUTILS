-- rather simplistic breaking of a string into substrings
-- does not handle escapes and quotes and such 
local cutils = require 'libcutils'
local function break_line(line, delim, ncols)
  assert(type(line) == "string")
  assert(type(delim) == "string", "delim = " .. delim)
  assert(#delim == 1)
  assert(type(ncols) == "number")
  assert(ncols >= 1)

  local cells = {}
  local start = 1
  for i = 1, ncols do 
    m, n = string.find(line, ",")
    if ( not m ) then -- eoln
      cells[i] = line
      break
    end 
    assert(n == m)
    local cell = string.sub(line, start, m-1)
    cells[i] = cell
    -- set up for next cell 
    line = string.sub(line, n+1, #line)
  end 
  return cells
end
return break_line 
--[[
local s = "abc,,def,ghi,,222"
local ncols = 6
local cells = break_line(s, ",", ncols)
for k, v in ipairs(cells) do print(k, v) end 
--]]
