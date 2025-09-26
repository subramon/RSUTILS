function ends_with(x, y)
  assert(type(x) == "string")
  assert(type(y) == "string")
  -- If y is an empty string, x always ends with y
  if y == "" then
    return true
  end

  -- If the length of y is greater than the length of x, x cannot end with y
  if #y > #x then
    return false
  end

  -- Extract the substring of x that has the same length as y, from the end of x
  local suffix = string.sub(x, -#y)

  -- Compare the extracted suffix with y
  return suffix == y
end
return ends_with
