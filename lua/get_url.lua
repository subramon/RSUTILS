--  sudo luarocks install Lua-cURL
local cURL = require("lcurl")
local cutils = require 'libcutils'
local function get_url(url, opfile)
  -- open output file
  f = assert(io.open(opfile, "w"))
  
  cURL.easy()
    -- setup url
    :setopt_url(url)
    -- setup file object as writer
    :setopt_writefunction(f)
    -- perform, invokes callbacks
    :perform()
  -- close easy
  :close()
  
  -- close output file
  f:close()
  return true
end
-- return get_url
-- quick and dirty test
local opfile = "_xx.html"
cutils.unlink(opfile)
assert(get_url("http://www.example.com", opfile))
assert(cutils.isfile(opfile))
cutils.unlink(opfile)
assert(get_url("https://www.google.com", opfile))
assert(cutils.isfile(opfile))
cutils.unlink(opfile)
print("done")
