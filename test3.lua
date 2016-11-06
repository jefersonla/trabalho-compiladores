-- Check conditional operators

-- if OK | else
if 0 == 0 then
    print(10)
else
    print(8)
end

-- if | else if | else if OK
if 10 > 100 then
    print(24)
elseif 10 ~= 10 then
    print(23)
elseif 10 >= 2 then
    print(11)
end

-- if | else OK
if 10 == 11 then
    print(24)
elseif 10 == 9 then
    print(24)
else
    print(12)
end
