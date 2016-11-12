-- Test functions OK
function why()
  while(0) do
    print(3)
  end
  print(11)
  z = 4
  while(z ~= 10) do
    print(2 + z)
    z = z + 1
  end
  local x
  print(x)
  x = 10
  print(x)
  for i = 10, 20 do
    print(i)
  end
  for i = 0, 20, 2 do
    print(i)
  end
end

why()

print(x)