local get_extension = require 'RSUTILS/lua/get_extension'
local x = get_extension("foo.csv")
assert(x == "csv")
local x = get_extension("foobarcsv")
assert(x == nil)
local x = get_extension("foobarcsv.")
assert(x == "")
print("Success for " .. arg[0])
