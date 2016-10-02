-- defines a factorial function
function expression (n, m)
  return n * m + 3
end
--!@"!@" -- Test with undefinied tokens is fine
--[[
	multiline comments
]]
print(expression(7, 10))