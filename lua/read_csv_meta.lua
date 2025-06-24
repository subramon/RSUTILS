local cutils       = require 'libcutils'
local ffi          = require 'ffi'
local stringify    = require 'RSUTILS/lua/stringify'
local tbl_to_C_2d  = require 'RSUTILS/lua/tbl_to_C_2d'
local x            = require 'RSUTILS/lua/qtypes_def';     ffi.cdef(x)
local x            = require 'RSUTILS/lua/csv_meta';       ffi.cdef(x)

-- this function is for call from load_configs() to work properly 
local function alt_read_csv_meta(
  cM
  )
  assert(type(glC) == "table")
  return read_csv_meta(glC, cM)
end

local function read_csv_meta(
  M,
  cM
  )
  assert(type(M) == "table")
  cM = ffi.cast("csv_meta_t *", cM)
  --====================================
  local col_names = {}
  local str_qtypes = {}
  local l_has_nulls = {}
  local l_is_load = {}
  local nC = #M -- number of columns in CSV files
  for i, m in ipairs(M) do 
    col_names[i] = m.name
    --======================================
    if (  m.is_load == nil ) then 
      m.is_load = true -- DEFAULT 
    end
    assert(type(m.is_load) == "boolean")
    l_is_load[i]    = m.is_load
    --======================================
    if ( not m.is_load ) then 
      m.qtype = "Q0"
    end
    assert(type(m.qtype) == "string")
    str_qtypes[i]    = m.qtype
    --======================================
    if (  m.has_nulls == nil ) then 
      m.has_nulls = false -- DEFAULT 
    end
    assert(type(m.has_nulls) == "boolean")
    l_has_nulls[i]    = m.has_nulls
  end
  cM[0].col_names,  cM[0].nC = tbl_to_C_2d(col_names)
  --== convert str_qtypes to C qtype 
  local x = ffi.C.malloc(nC * ffi.sizeof("int"))
  ffi.fill(x, nC * ffi.sizeof("int"), 0)
  x = ffi.cast("int *", x)
  for i = 1, nC do 
    x[i-1] = cutils.get_c_qtype(str_qtypes[i])
  end
  cM[0].qtypes = x
  --===================================
  --== convert l_has_nulls to C has_nulls
  local x = ffi.C.malloc(nC * ffi.sizeof("bool"))
  ffi.fill(x, nC * ffi.sizeof("bool"), 0)
  x = ffi.cast("bool *", x)
  for i = 1, nC do 
    x[i-1] = l_has_nulls[i]
  end
  cM[0].has_nulls = x
  --===================================
  --== convert l_is_load to C is_load
  local x = ffi.C.malloc(nC * ffi.sizeof("bool"))
  ffi.fill(x, nC * ffi.sizeof("bool"), 0)
  x = ffi.cast("bool *", x)
  for i = 1, nC do 
    x[i-1] = l_is_load[i]
  end
  cM[0].is_load = x
  --===================================
  --== convert str_qtypes to C width
  local x = ffi.C.malloc(nC * ffi.sizeof("uint32_t"))
  ffi.fill(x, nC * ffi.sizeof("uint32_t"), 0)
  x = ffi.cast("uint32_t *", x)
  for i = 1, nC do 
    x[i-1] = cutils.get_width_qtype(str_qtypes[i])
  end
  cM[0].widths = x
  --===================================
  --== convert str_qtypes to C formats
  local x = ffi.C.malloc(nC * ffi.sizeof("char *"))
  ffi.fill(x, nC * ffi.sizeof("char *"), 0)
  x = ffi.cast("char **", x)
  for i = 1, nC do 
    local lfmt = cutils.get_format(str_qtypes[i])
    if ( lfmt ~= nil ) then 
      assert(type(lfmt) == "string")
      local len = #lfmt+1
      local cfmt = ffi.C.malloc(len)
      ffi.fill(cfmt, len, 0)
      ffi.copy(cfmt, lfmt)
      x[i-1] = cfmt
    end
  end
  cM[0].formats = x
  return true
end
return read_csv_meta 
