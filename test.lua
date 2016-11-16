function z(y)
    local x = 1
    print(x)
    while 1 do
        print(x)
        if x >= 10 then
            return x
        end
        if x > y then
            y = 2 * x
        end
        x = x + y
    end
    return 0
end

print(z(1))