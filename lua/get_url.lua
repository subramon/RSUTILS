--  sudo luarocks install Lua-cURL
local cURL = require("lcurl")
local function get_url(url, opfile)
  -- open output file
  
  local y = ""
  local function to_str(x) y = y .. x end 
  cURL.easy()
    -- setup url
    :setopt_url(url)
    -- setup file object as writer
    :setopt_writefunction(to_str)
    -- perform, invokes callbacks
    :perform()
  -- close easy
  :close()
  
  return y
end
return get_url
-- quick and dirty test
--[[
local x = assert(get_url("http://www.example.com", opfile))
print(type(x))
print(#x)
print(x)
print("done")
--]]
