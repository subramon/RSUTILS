-- Assume 'json' is a JSON library loaded in your environment.
-- For example, you might use:

-- local simdjson = require 'lua-simdjson'
local JSON = require 'RSUTILS/lua/JSON'
local is_in = require 'RSUTILS/lua/is_in'

-- Given an array "data"
-- where each element is a table of the form { x = y, .... }
-- create an HTML representation
function df_tbl_to_html(data)
  assert(type(data) == "table")
  local T = {} -- output 
  assert(#data > 0)
  local K = {} -- names of columns in HTML table 
  for k, v in ipairs(data) do 
    assert(type(v) == "table")
    for k2, v2 in pairs(v) do
      if ( not is_in(k2, K) ) then
        K[#K+1] = k2
      end
    end
  end
  assert(#K > 0)
  -- print preamble 
  T[#T+1] = [[
  <!DOCTYPE html>
<html>
<body>
  <center>
<style>
    table, th, td {
  border: 1px solid black;
}
</style>
<body>
<table>
]]
  -- print header line
  T[#T+1] = "<thead><tr> "
  for _, k in ipairs(K) do 
    T[#T+1] = "<td> "
    T[#T+1] = k
    T[#T+1] = "</td> "
  end
  T[#T+1] = "</tr></thead>"

  -- Create the table body (<tbody>) with rows (<tr>) and data cells (<td>)
  T[#T+1] = "<tbody>"
  for k, v in pairs(data) do 
    T[#T+1] = "<tr>"
    for k2, v2 in pairs(K) do 
      T[#T+1] = "<td>"
      if ( v[v2] ) then 
        T[#T+1] = tostring(v[v2])
      else
        T[#T+1] = " -- "
      end
      T[#T+1] = "</td>"
    end
    T[#T+1] = "</tr>"
  end
  T[#T+1] = "</tbody>"
  -- footer
  T[#T+1] = "</table> </body></html>"

  return table.concat(T, "")
end

local is_test = false
if ( not is_test ) then
  return df_tbl_to_html
else 
  -- Example Usage:
  -- Assuming you have a JSON string like:
  local json_string = '[{"name": "John", "age": 30}, {"name": "Jane", "age": 25}]'
  local data_table = JSON:decode(json_string)  -- Use your JSON library to decode
  
  local html_table = df_tbl_to_html(data_table)
  print(html_table)
end
