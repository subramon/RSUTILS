local read_csv_meta = require 'RSUTILS/lua/read_csv_meta'

-- this function is for call from load_configs() to work properly 
local function alt_read_csv_meta(
  cM
  )
  assert(type(glC) == "table")
  return read_csv_meta(glC, cM)
end
return alt_read_csv_meta
