local ffi         = require 'ffi'
local cutils      = require 'libcutils'
local x           = require 'web_info';  ffi.cdef(x)
local stringify   = require 'stringify'
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
  local login_page = glC.login_page
  if ( login_page ~= nil ) then 
    assert(type(login_page) == "string")
    assert(#login_page > 0)
    cC[0].login_page = stringify(login_page) 
  end
  --====================================
  local login_endp = glC.login_endp
  if ( login_endp ~= nil ) then 
    assert(type(login_endp) == "string")
    assert(#login_endp > 0)
    cC[0].login_endp = stringify(login_endp) 
  end
  --====================================
  local docroot = glC.docroot
  if ( docroot ~= nil ) then 
    assert(type(docroot) == "string")
    assert(#docroot > 0)
    assert(cutils.isdir(docroot))
    cC[0].docroot = stringify(docroot) 
  end
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
  for _, u in ipairs(users) do
    assert(#u <= 32-1) -- TODO Document this 
  end
  cC[0].users, cC[0].n_users = tbl_to_C_2d(users)
  --====================================
  local n_threads = glC.n_threads
  if ( n_threads == nil ) then n_threads = 1 end
  assert(type(n_threads) == "number")
  assert(n_threads > 0)
  cC[0].n_threads = n_threads 
  --====================================
  local n_sessions = glC.n_sessions
  if ( n_sessions == nil ) then n_sessions = 1 end
  assert(type(n_sessions) == "number")
  assert(n_sessions > 0)
  cC[0].n_sessions = n_sessions 
  --====================================
  local timeout_sec = glC.timeout_sec
  if ( timeout_sec == nil ) then timeout_sec = 30*86400 end
  assert(type(timeout_sec) == "number")
  assert(timeout_sec > 0)
  cC[0].timeout_sec = timeout_sec 
  --====================================
  local init_lua_state = glC.init_lua_state
  if ( init_lua_state ~= nil ) then 
    assert(type(init_lua_state) == "string")
    assert(#init_lua_state > 0)
    cC[0].init_lua_state = stringify(init_lua_state)
  end
  --====================================
  cC[0].sess_state = ffi.C.malloc(#users * ffi.sizeof("sess_state_t"))
  ffi.fill(cC[0].sess_state, (#users * ffi.sizeof("sess_state_t")), 0)
  --====================================
  return true
end
return read_web_info 
