local x 
print(x)        -- Should print nil
x = 10
print(x)        -- Should print 10

do
    
    print(x)    -- Should print 10
    local x
    print(x)    -- Should print nil
    x = 20
    print(x)    -- Should print 20
    
end
print(x)        -- Should print 10

-- Separator
print(987654321)

if 1 then
    local x
    print(x)
    x = 40
    print(x)
end

print(x)

-- Separator
print(123456789)

function test()
    local x
    print(x)    -- Should print nil
    x = 30
    print(x)    -- Should print 30
end

test()

print(x)        -- Should print 10
