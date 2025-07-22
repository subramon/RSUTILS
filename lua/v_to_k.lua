local function v_to_k(v, V)
  assert(type(v) == "string")
  assert(type(V) == "table")
  local k = -1
  assert(#V > 0)
  for k1, v1 in ipairs(V) do 
    if ( v1 == v ) then
      k = k1
      break
    end
  end
  return k
end
return v_to_k
