local ffi = require 'ffi'
ffi.cdef([[
extern       void *malloc(size_t size);
extern       void free(void *ptr);
extern       void *calloc(size_t nmemb, size_t size);
extern       void *realloc(void *ptr, size_t size);
extern       void *reallocarray(void *ptr, size_t nmemb, size_t size);
]]
)
local read_csv_meta = require 'RSUTILS/lua/read_csv_meta'
-- start sample data 
local M = {}
M[#M+1] = { name = "slice", qtype = "I1", has_nulls = false, is_load = false, } 
M[#M+1] = { name = "tcin", qtype = "SC:16", has_nulls = false, } -- SCLR for for Pass1, VCTR for Pass 2 
M[#M+1] = { name = "style_c", qtype = "SC:16", is_load = false, has_nulls = false, } 
M[#M+1] = { name = "sub_class_c", qtype = "SC:16", is_load = false, has_nulls = false, } 
M[#M+1] = { name = "item_cluster_c", qtype = "SC:16", is_load = false, has_nulls = false, } 
M[#M+1] = { name = "base_week", qtype = "TM1:%Y-%m-%d", has_nulls = true,  } --NOT in historical 
M[#M+1] = { name = "size_per_unit_q", qtype = "F4", has_nulls = true, } -- DELETE 
M[#M+1] = { name = "holiday_array", qtype = "SC:36", has_nulls = true,  } -- VCTR
M[#M+1] = { name = "days_until_christmas", qtype = "I4", has_nulls = true, }  -- VCTR 
M[#M+1] = { name = "holiday_weeks_christmas", qtype = "I4", has_nulls = false, }  -- VCTR
M[#M+1] = { name = "sls_unit_q", qtype = "F4", has_nulls = true, } -- VCTR
M[#M+1] = { name = "on_promo_flag", qtype = "F4", has_nulls = false, } -- VCTR
M[#M+1] = { name = "non_pbo_promo_flag", qtype = "BL", has_nulls = false, } -- VCTR
M[#M+1] = { name = "stockout_flag", qtype = "F4", has_nulls = true, } -- VCTR
M[#M+1] = { name = "n_stores", qtype = "F4", has_nulls = false, } -- VCTR
M[#M+1] = { name = "regular_price", qtype = "F4", has_nulls = true, } -- VCTR
M[#M+1] = { name = "selling_price", qtype = "F4", has_nulls = true, } -- VCTR
M[#M+1] = { name = "week_start_date", qtype = "TM1:%Y-%m-%d", has_nulls = false,  }
M[#M+1] = { name = "bts", qtype = "I4", has_nulls = false, } -- VCTR
M[#M+1] = { name = "btc", qtype = "I4", has_nulls = false, } -- VCTR
M[#M+1] = { name = "btcs", qtype = "I4", has_nulls = false, } -- VCTR
M[#M+1] = { name = "model_config_tag", qtype = "SC:20", has_nulls = false, }  -- SCLR
M[#M+1] = { name = "beta", qtype = "F4", has_nulls = false, } -- SCLR
M[#M+1] = { name = "alpha", qtype = "F4", has_nulls = false, } -- SCLR
M[#M+1] = { name = "max_promo_lift", qtype = "F4", has_nulls = false, } -- OMAR
M[#M+1] = { name = "sp_cs", qtype = "F4", has_nulls = false, } --VCTR
M[#M+1] = { name = "upper_threshold", qtype = "F4", has_nulls = true, } -- FWD ONLY 
M[#M+1] = { name = "upper_replacement", qtype = "F4", has_nulls = true, } -- FWD ONLY 
M[#M+1] = { name = "method", qtype = "SC:4", has_nulls = true, } -- FWD ONLY 
M[#M+1] = { name = "grain", qtype = "SC:4", has_nulls = true, } -- FWD ONLY 
-- stop sample data 

local tests = {}
tests.t1 = function()
  local cM = ffi.C.malloc(ffi.sizeof("csv_meta_t"))
  ffi.fill(cM, ffi.sizeof("csv_meta_t"), 0)
  cM = ffi.cast("csv_meta_t *", cM)
  assert(read_csv_meta(M, cM))
  for i = 1, cM[0].nC do 
    print("Column " .. i .. " " .. ffi.string((cM[0].col_names[i-1])))
    ffi.C.free(cM[0].col_names[i-1])
    ffi.C.free(cM[0].formats[i-1])
  end
  ffi.C.free(cM[0].col_names)
  ffi.C.free(cM[0].widths)
  ffi.C.free(cM[0].formats)
  ffi.C.free(cM[0].qtypes)
  ffi.C.free(cM[0].has_nulls)
  ffi.C.free(cM[0].is_load)
  ffi.C.free(cM)
  print("Success on test t1 ");
end
-- return tests
tests.t1()
