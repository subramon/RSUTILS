local cutils = require 'libcutils'
local is_in = require 'RSUTILS/lua/is_in'
local ok_chars = { 
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", 
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", 
  "_", ".", "-", }

local function no_funny_chars(str, addnl)
  assert(type(str) == "string")
  for i = 1, #str do
     local c = string.sub(str, i, i)
     if ( ( is_in(c, ok_chars) ) or 
        ( ( type(addnl) == "table") and is_in(c, addnl) ) ) then
        -- all is well
      else
       return false
     end
   end
   return true
end
local function mk_button(I)
  assert(type(I) == "table")
  local X = {}
  X[#X+1] = "<button "
  X[#X+1] = ' type="button" '
  --=== handle id 
  local id 
  if ( I.id ) then 
    assert(type(I.id) == "string")
    assert(no_funny_chars(I.id))
    X[#X+1] = string.format('id = "%s" ', I.id)
  end 
  --=== handle endpoint
  local name 
  if ( I.endpoint ) then  -- TODO Is this compulsory
    assert(type(I.endpoint) == "string")
    assert(no_funny_chars(I.endpoint))
    X[#X+1] = string.format('data-endpoint = "/%s" ', I.endpoint)
  end 
  X[#X+1] = ' onclick="submitAjax(this)" '
  X[#X+1] = " > "
  assert(type(I.button_string) == "string")
  assert(no_funny_chars(I.button_string, { " ", }))
  X[#X+1] = I.button_string
  X[#X+1] = "</button>"
  return table.concat(X, "")
end
--[[ UNIT TEST 
return  mk_button
--]]
local I = {}
I.id = "button_id_xx"
I.button_string = "Submit button"
I.endpoint = "Ignore"
print(mk_button(I))

--[[
-- WHAT ABOUT 
--               <div class="status-ajax" id="%sStatus"></div>
--
--DISCUSS WITH UTPAL
--data-endpoint="/Set%s"
A custom HTML5 data attribute. It stores extra, non-visual data—in this case the API path the button should hit. JavaScript can read it via el.dataset.endpoint.

--type="button"
Makes it a plain button (not a form submitter). If it were inside a <form>, it would not submit the form automatically—JavaScript will handle the action.

onclick="submitAjax(this)"
An inline event handler. When the button is clicked, it calls a global JS function submitAjax(...), passing this (the button element) so the function can read its attributes (like dataset.endpoint) or find related DOM nodes.

--]]

