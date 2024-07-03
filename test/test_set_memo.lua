local qcfg    = require 'Q/UTILS/lua/qcfg'
local Q       = require 'Q'
local lVector = require 'Q/RUNTIME/VCTR/lua/lVector'

local tests = {}

tests.t1 = function()
  local vec1 = lVector({qtype = "I4"})
  assert(vec1:is_memo() == true) -- default value of qconsts.is_memo is true
  Q.set_memo(false) -- value of qconsts.is_memo set to false
  local vec2 = lVector({qtype = "I4"})
  assert(vec2:is_memo() == false)
  local vec3 = lVector({qtype = "I4", is_memo = true})
  assert(vec3:is_memo() == true) -- is_memo field in vec args has priority
  print("Successfully completed test t1")
end

tests.t2 = function()
  assert(Q.set_memo(true))
  assert(qcfg.is_memo == true)

  assert(Q.set_memo(false))
  assert(qcfg.is_memo == false)

  assert( not Q.set_memo("false"))

  print("Successfully completed test t2")
end
-- tests.t2()
return tests
