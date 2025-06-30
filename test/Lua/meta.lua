local M = {}
M[#M+1] = { name = "slice", qtype = "I1", has_nulls = false, is_load = false, } 
M[#M+1] = { name = "tcin", qtype = "SC:16", has_nulls = false, } 
M[#M+1] = { name = "style_c", qtype = "SC:16", is_load = false, has_nulls = false, } 
M[#M+1] = { name = "sub_class_c", qtype = "SC:16", is_load = false, has_nulls = false, } 
M[#M+1] = { name = "item_cluster_c", qtype = "SC:16", is_load = false, has_nulls = false, } 
M[#M+1] = { name = "base_week", qtype = "TM1:%Y-%m-%d", has_nulls = true,  }
M[#M+1] = { name = "size_per_unit_q", qtype = "F4", has_nulls = true, } 
M[#M+1] = { name = "holiday_array", qtype = "SC:36", has_nulls = true,  }
M[#M+1] = { name = "days_until_christmas", qtype = "I4", has_nulls = true, } 
M[#M+1] = { name = "holiday_weeks_christmas", qtype = "I4", has_nulls = false, } 
M[#M+1] = { name = "sls_unit_q", qtype = "F4", has_nulls = true, } 
M[#M+1] = { name = "on_promo_flag", qtype = "F4", has_nulls = false, } 
M[#M+1] = { name = "non_pbo_promo_flag", qtype = "BL", has_nulls = false, } 
M[#M+1] = { name = "stockout_flag", qtype = "F4", has_nulls = true, } 
M[#M+1] = { name = "n_stores", qtype = "F4", has_nulls = false, } 
M[#M+1] = { name = "regular_price", qtype = "F4", has_nulls = true, } 
M[#M+1] = { name = "selling_price", qtype = "F4", has_nulls = true, } 
M[#M+1] = { name = "week_start_date", qtype = "TM1:%Y-%m-%d", has_nulls = false,  }
M[#M+1] = { name = "bts", qtype = "I4", has_nulls = false, } 
M[#M+1] = { name = "btc", qtype = "I4", has_nulls = false, } 
M[#M+1] = { name = "btcs", qtype = "I4", has_nulls = false, } 
M[#M+1] = { name = "model_config_tag", qtype = "SC:28", has_nulls = false, } 
M[#M+1] = { name = "beta", qtype = "F4", has_nulls = false, } 
M[#M+1] = { name = "alpha", qtype = "F4", has_nulls = true, }  -- TODO P1
M[#M+1] = { name = "max_promo_lift", qtype = "F4", has_nulls = true, }  -- TODO P1 
M[#M+1] = { name = "sp_cs", qtype = "F4", has_nulls = true, }  -- TODO P1 
return M
