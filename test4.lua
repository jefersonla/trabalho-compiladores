--local x = 0
--print(x)
--print (x + 1)

-- For seems to be OK
for i = 0, 10 do
    print((i + 2) * 2)
    if i == 0 then
        i = 4
    end
    print(i)
    print(1222333444)
end

function z()
    for i = 0, 10 do
        print(i)
        
        if i == 5 then
            return
        end
    end
end

z()