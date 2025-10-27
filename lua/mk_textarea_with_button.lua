local cutils = require 'libcutils'
local is_in = require 'RSUTILS/lua/is_in'
local no_funny_chars = require 'RSUTILS/lua/no_funny_chars'
local mk_textarea = require 'RSUTILS/lua/mk_textarea'
local mk_button = require 'RSUTILS/lua/mk_button'
local function mk_textarea_with_button(disp_str, aux, taux, baux)
  local X = {}
  assert(type(aux) == "table")
  -- aux is for combination
  -- taux is for textarea
  -- baux is for button
  -- aux.id is used to connect textarea with button with diagnostic message
  assert(type(aux.id) == "string")
  assert(#aux.id > 0)
  assert(no_funny_chars(aux.id))
  -- first generate textarea
  if ( not taux ) then taux = {} end 
  assert(type(taux) == "table")
  --=========================================
  if ( not taux.id ) then 
    taux.id   = aux.id .. "Input" 
  end 
  assert(type(taux.id) == "string")
  assert(#taux.id > 0)
  --=========================================
  if ( not taux.name ) then 
    taux.name = aux.id 
  end 
  assert(type(taux.name) == "string")
  assert(#taux.name > 0)
  --=========================================
  X[#X+1] = mk_textarea(disp_str, taux)
  if ( not aux.is_button ) then return table.concat(X, "") end 
  -- control here => button is wanted 
  if ( aux.button_below_textarea ) then 
    assert(type(aux.button_below_textarea) == "boolean")
    X[#X+1] = "<br>"
  end
  -- generate button 
  if ( not baux ) then baux = {} end 
  baux.id   = aux.id .. "Submit" 
  --========================================
  if ( not(baux.method) ) then 
    baux.method = "GET"
  end
  assert(type(baux.method) == "string")
  assert(is_in(baux.method, {"GET", "POST"}))
  --========================================
  if ( not(baux.endpoint) ) then 
    baux.endpoint = "Set" .. aux.id 
  end
  assert(type(baux.endpoint) == "string")
  assert(#baux.endpoint > 0)
  --========================================
  if ( not(baux.onclick) ) then 
    baux.onclick = "submitAjax((this)"
  end
  assert(type(baux.onclick) == "string")
  assert(#baux.onclick > 0)
  --========================================
  if ( not(baux.button_string) ) then 
    baux.button_string = "Submit " .. aux.id
  end
  assert(type(baux.button_string) == "string")
  assert(#baux.button_string > 0)
  --========================================
  X[#X+1] = mk_button(baux)
  --========================================
  if ( baux.debug ) then 
    if ( not baux.debug_id )  then 
      baux.debug_id = aux.id .. "Status"
    end 
    assert(type(baux.debug_id) == "string")
    assert(#baux.debug_id > 0)
    local x = string.format('<div class="status-ajax" id = "%s"></div>',
    baux.debug_id)
    X[#X+1] = x
  end
  local str = table.concat(X, "")
  return str
end
return mk_textarea_with_button

--[[ UNIT TEST 

local aux = {}
aux.id = "Model"
aux.is_button = true 
aux.button_below_textarea = true
local taux = {}
local disp_str = "This is a text area "
taux.rows = 10
taux.cols = 20
taux.minlength = 30
taux.maxlength = 40
taux.readonly = true 
taux.wrap = "soft"
taux.placeholder = "placeholder xx"

local baux = {}
baux.debug = true

print(mk_textarea_with_button(disp_str, aux, taux, baux))


--]]
