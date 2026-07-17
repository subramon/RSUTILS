local cutils = require 'libcutils'
local break_line = require 'RSUTILS/lua/break_line'
local is_unique = require 'RSUTILS/lua/is_unique'
local function csv_to_vecs(infile, delim, col_names, is_hdr)

  assert(type(infile) == "string")
  assert(cutils.isfile(infile))
  assert(type(delim) == "string")
  assert(#delim == 1)
  assert(type(is_hdr) == "boolean")

  assert(type(col_names) == "table")
  local ncols = #col_names
  assert(ncols >= 1)
  for k, v in pairs(col_names) do assert(type(v) == "string") end 
  assert(is_unique(col_names))

  local nrows = 0
  local T = {} -- to be returned
  for k, v in pairs(col_names) do T.v = {} end 
  -- Loop through each line of the file safely
  local lno = 1
  for line in io.lines(infile) do
    if ( ( is_hdr ) and ( lno == 1 ) ) then 
      -- skip this line
    else
      nrows = nrows + 1 
      -- break line into cells 
      local cells = break_line(line, delim, ncols)
      -- assign each cell to corresponding vector 
      for k, v in pairs(col_names) do 
        T.v[nrows] = cells[k]
      end 
    end
    lno = lno + 1 
  end
  return T, nrows
end
return csv_to_vecs
