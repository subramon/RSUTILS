local read_csv_spec = require 'RSUTILS/lua/read_csv_spec'

-- this function is for call from load_configs() to work properly 
local function alt_read_csv_spec(
  cM
  )
  assert(type(glC) == "table")
  return read_csv_spec(glC, cM)
end
return alt_read_csv_spec
