local cutils = require 'libcutils'
local x = cutils.ls("/tmp/", "files", ".*.c$")
local x = cutils.ls("/tmp/", "files", ".*.json$")
for k, v in pairs(x) do print(k, v) end
print("All done")
