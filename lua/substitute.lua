local function substitute(infile, instr, outstr, outfile)
  --== a few basic checks
  assert(type(infile) == "string")
  assert(type(instr) == "string")
  assert(type(outfile) == "string")
  assert(type(outstr) == "string")
  assert(cutils.isfile(infile)
  assert(infile ~= outfile)
  assert(#instr > 0)
  --======================================
  local ifp = assert(io.open(infile, "rb"))
  local in_content = ifp:read("*all")
  ifp:close()
  local out_content = string.gsub(in_content, instr, outstr)

  ofp = io.open(outfile, "w")
  ofp:write(out_content)
  ofp:close()
  return true
end
