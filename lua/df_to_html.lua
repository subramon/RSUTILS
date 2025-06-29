-- Assume 'json' is a JSON library loaded in your environment.
-- For example, you might use:

-- local simdjson = require 'lua-simdjson'
local JSON = require 'RSUTILS/lua/JSON'

-- Given an array "data"
-- each 
--   key is a string
--   value is an array
-- create an HTML representation
function df_tbl_to_html(data)
  assert(type(data) == "table")
  assert(#data > 0)
  for k, v in ipairs(data) do 
    assert(type(v) == "table")
  end
  local html = [[
  <!DOCTYPE html>
<html>
<body>
  <center>
<style>
    table, th, td {
  border: 1px solid black;
}
</style>
]]
  html = html .. "<table>"
  html = html .. "<body>"

  -- Assume the data is a table of tables (an array of objects in JSON)
  -- Create the table header (<th>) using the keys from the first row
  if data and data[1] and type(data[1]) == "table" then
    html = html .. "<thead><tr>"
    for key, _ in pairs(data[1]) do
      html = html .. "<th>" .. tostring(key) .. "</th>"
    end
    html = html .. "</tr></thead>"

    -- Create the table body (<tbody>) with rows (<tr>) and data cells (<td>)
    html = html .. "<tbody>"
    for _, row_data in ipairs(data) do
      html = html .. "<tr>"
      for key, value in pairs(row_data) do
        html = html .. "<td>" .. tostring(value) .. "</td>"
      end
      html = html .. "</tr>"
    end
    html = html .. "</tbody>"
  else
    -- Handle cases where the data is not a table of tables or is empty
    html = html .. "<tr><td>No data available or invalid format.</td></tr>"
  end

  html = html .. "</table>"
  html = html .. "</body>"
  html = html .. "</html>"
  return html
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
