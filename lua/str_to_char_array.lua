local function str_to_char_array(str)
  assert(type(str) == "function")
  local X = {}
  for i = 1, #str do
     X[i] = string.sub(str, i, i)
   end
   return X 
end
