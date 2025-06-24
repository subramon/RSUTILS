local ffi = require 'ffi'
local tbl_to_C_2d = require 'RSUTILS/lua/tbl_to_C_2d'
local stringify   = require 'RSUTILS/lua/stringify'
local x           = require 'RSUTILS/lua/csv_spec'; ffi.cdef(x)

-- lS is the spec in Lua
-- cS is the spec in C 
local function read_csv_spec(lS, cS)
  if ( not M ) then M = glC end
  cS = ffi.cast("csv_spec_t *", cS)
  local T, nT = tbl_to_C_2d(lS.break_cols)
  cS[0].break_cols = T
  cS[0].nB = nT
  local is_hdr = false
  if ( type(lS.is_hdr) == "nil" ) then
    is_hdr = false
  else
    assert(type(lS.is_hdr) == "boolean")
    is_hdr = lS.is_hdr
  end
  cS[0].is_hdr = is_hdr
  --===================================
  if ( lS.fld_sep == "comma" ) then 
    cS[0].str_fld_sep = stringify(",")
  elseif ( lS.fld_sep == "ctrl_a" ) then 
    error("TODO")
  else
    error("NOT IMPLEMENTED")
  end
  --===================================
  if ( lS.fld_delim == "dquote" ) then 
    cS[0].str_fld_delim = stringify('"')
  elseif ( lS.fld_delim == 1 ) then 
    error("TODO")
  else
    error("NOT IMPLEMENTED")
  end
  --===================================
  if ( lS.rec_sep == "eoln" ) then 
    cS[0].str_rec_sep = stringify("\n")
  else
    error("NOT IMPLEMENTED")
  end
  --===================================
  return true
end 
return read_csv_spec
