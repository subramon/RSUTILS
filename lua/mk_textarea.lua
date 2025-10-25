local tmpl1 = [[
              <textarea %s id="%sInput" name="%s" rows="%d" cols="%d">
]]
local tmpl2 = [[
              <button type="button" id="%sSubmit" data-endpoint="/Set%s"
                onclick="submitAjax(this)">Submit %s</button>
              <div class="status-ajax" id="%sStatus"></div>
]]
local function  mk_textarea(disp_str, aux)
  assert(type(disp_str) == "string")
  assert(type(aux) == "table")
  local X = {}
  X[#X+1] = "<textarea "
  X[#X+1] = "></textarea>> "
  --=== handle rows 
  local nrows 
  if ( aux.nrows ) then 
    nrows = aux.nrows 
    assert(type(nrows) == "number")
    assert(#nrows >= 1)
  end 
  if ( nrows ) then 
    X[#X+1] = string.format('rows = "%d" ', nrows)
  end
  --=== handle cols
  local ncols 
  if ( aux.ncols ) then 
    ncols = aux.ncols 
    assert(type(ncols) == "number")
    assert(#ncols >= 1)
  end 
  if ( ncols ) then 
    X[#X+1] = string.format('cols = "%d" ', ncols)
  end
  --=== handle id 
  local id 
  if ( aux.id ) then 
  end 
  --=== handle placeholder 


  local ncols = 50 -- default 
  if ( aux ) then 
    assert(type(aux) == "table") 
    if ( aux.nrows ) then
      assert(type(aux.nrows) == "number")
      assert(aux.nrows > 0)
      nrows = aux.nrows
    end
    if ( aux.ncols ) then
      assert(type(aux.ncols) == "number")
      assert(aux.ncols > 0)
      ncols = aux.ncols
    end
  end 
  local read_only = ""
  if ( state.mode == "Original" ) then 
    read_only = "readonly"
  end 
  local tab = state.tab
  local X = {}
  X[#X+1] = string.format(tmpl1, read_only, tab, tab, nrows, ncols)
  X[#X+1] = cutils.html_escape(disp_str)
  X[#X+1] = "</textarea>"
  if ( state.mode == "Modified" ) then 
    X[#X+1] = string.format(tmpl2, tab, tab, tab, tab)
  end
  X[#X+1] = " </div>"
  str = table.concat(X, "")
  return str
end
return  mk_disp_textarea
