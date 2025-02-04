local ffi       = require 'ffi'
local cutils    = require 'libcutils'
local x         = require 'web_info';  ffi.cdef(x)
local tbl_to_C_2d = require 'tbl_to_C_2d'

local function read_web_info(
  cC -- this is struct with C  configs
  )
  cC = ffi.cast("web_info_t *", cC)
  assert(type(glC) == "table") -- this must exist in Lua state 
  --====================================
  local port = assert(glC.port)
  assert(type(port) == "number")
  assert(port > 0)
  cC[0].port = port 
  --====================================
  local is_external = glC.is_external
  if ( is_external == nil ) then is_external = false end
  assert(type(is_external) == "boolean")
  cC[0].is_external = is_external 
  --====================================
  local max_headers_size = glC.max_headers_size
  if ( max_headers_size == nil ) then max_headers_size = 0 end
  assert(type(max_headers_size) == "number")
  assert(max_headers_size >= 0)
  cC[0].max_headers_size = max_headers_size 
  --====================================
  local max_body_size = glC.max_body_size
  if ( max_body_size == nil ) then max_body_size = 0 end
  assert(type(max_body_size) == "number")
  assert(max_body_size >= 0)
  cC[0].max_body_size = max_body_size 
  --====================================
  local users = assert(glC.users)
  assert(type(users) == "table")
  assert(#users > 0)
  cC[0].users, cC[0].n_users = tbl_to_C_2d(users)
  --====================================
  local n_sessions = glC.n_sessions
  if ( n_sessions == nil ) then n_sessions = 1 end
  assert(type(n_sessions) == "number")
  assert(n_sessions > 0)
  --====================================
  return true
end
return read_web_info 
