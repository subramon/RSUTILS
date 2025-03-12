function get_extension(x)
  assert(type(x) == "string")
  assert(#x >= 3)
  local dotpos = -1
  for i = x:len(), 1, -1 do
    if x:sub(i,i) == "." then
      dotpos = i
      break
    end
  end
  if ( dotpos < 1 ) then return nil end
  return string.sub(x, dotpos+1)
end
return  get_extension
