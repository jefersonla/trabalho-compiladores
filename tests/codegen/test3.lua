-- Check conditional operators

-- if - OK | else
if 0 == 0 then
    print(10)
else
    print(8)
end
-- 10

-- if | else if | else if - OK
if 10 > 100 then
    print(24)
elseif 10 ~= 10 then
    print(23)
elseif 10 >= 2 then
    print(11)
end
-- 11

-- if | else - OK
if 10 == 11 then
    print(24)
elseif 10 == 9 then
    print(24)
else
    print(12)
end
-- 12

-- Print separator
print(1212121212)

-- Testing store instruction - OK
x = 5
print(x)

-- Change the variable and testing new value - OK
x = 10
print(x)

-- Nil global variable - OK
print(y)

-- Load instruction - OK
y = x + 10
print(y)

-- Print separator
print(1313131313)

-- While with variables - OK
while x ~= 0 do
    print(10)
    x = x - 1
end

-- Print separator
print(1414141414)

-- Testing while and if concatenated - OK
if x == 0 then
    x = 12
    while x < 20 do
        print(x)
        x = x + 2
    end
elseif x == 2 then
    print(24)
elseif x == 340 then
    print(240)
else
    print(2400)
end

-- Print separator
print(1515151515)

-- Testing multiple assigns - OK
x = 1
x, y = 10, 20
print(x)
print(y)
y = 30
print(y)
y = nil
print(y)