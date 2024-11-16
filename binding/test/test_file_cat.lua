local cutils = require 'libcutils'
cutils.str_as_file("abc", "_x")
cutils.str_as_file("def", "_y")
cutils.file_cat("_x", "_y")
local chk = cutils.file_as_str("_x")
assert(chk == "abcdef")
cutils.delete("_x")
cutils.delete("_y")
print("SUCCESS")
