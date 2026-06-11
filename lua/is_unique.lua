local function is_unique ( X)
  local T = {}
  assert(type(X) == "table")
  for _, v in pairs(X) do 
    assert((type(v) == "number") or 
           (type(v) == "string"))
    assert(not T[v])
    T[v]= true
  end
  return true 
end
return is_unique
