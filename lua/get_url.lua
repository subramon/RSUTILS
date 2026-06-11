--  sudo luarocks install Lua-cURL
-- local cURL = require("lcurl")
local curl = require("cURL")
local function get_url(url, cookie_file, cookie_jar)
  
  -- Initialize curl easy session
  local c = curl.easy()
  
  -- Enable cookie engine and load existing cookies
  if ( cookie_file ) then 
    c:setopt(curl.OPT_COOKIEFILE, cookie_file)
  end 
  
  -- Save cookies to file when the session closes
  if ( cookie_jar ) then 
    c:setopt(curl.OPT_COOKIEJAR, cookie_jar)
  end
  
  -- Set target URL
  c:setopt(curl.OPT_URL, url)
  
  -- Perform the request
  local success, err = pcall(function()
      c:perform()
  end)
  
  if not success then
      print("Curl error: ", err)
  end
  
  -- Cookies are written to the file during close/cleanup
  c:close()
end
return get_url
--[[
-- quick and dirty test
url = "http://www.example.com"
url = "http://localhost:8080/Login?User=subramon"
cookie_file = "_cookies.txt"
cookie_jar = "_cookies.txt"
rslt = get_url(url, cookie_file, cookie_jar)
print(rslt)
--]]

