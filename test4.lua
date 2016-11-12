--local x = 0
--print(x)
--print (x + 1)

-- 'For' seems to be OK
for i = 0, 10 do
    print((i + 2) * 2)
    if i == 0 then
        i = 4
    end
    print(i)
    print(1222333444)
end

-- For with increment
for i = 0, 10, 3 do
    print(i)
end

print(1333444222)

x = 2

-- for with variable increment
-- this should print 0 4 and 10
for i = 0, 10, x do
    print(i)
    if x < 10 then
        x = x + 2
    end
end

print(151553444)

-- This should print only from 0 to 5
function z()
    for i = 0, 10 do
        print(i)
        
        if i == 5 then
            return
        end
    end
end

z()

-- For just a convention this is NIL! so if this number is used
print(2147483641) -- SHOULD BE PRINTED ... but it will only display nil :/