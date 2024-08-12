local ffi = require 'ffi'
local tbl_to_C_2d = require 'RSUTILS/lua/tbl_to_C_2d'
local x, n = tbl_to_C_2d({"abc", "def"})
assert(n == 2)
s1 = ffi.string(x[0])
s2 = ffi.string(x[1])
assert(s1 == "abc")
assert(s2 == "def")
print("Completed test_tbl_to_C_2d successfully")
