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
    print(x)    -- Should print nil
    x = 40
    print(x)    -- Should print 40
end

print(x)        -- Should print 10

-- Separator
print(123456789)

z = 1

while z ~= 2 do
    z = 2
    local x
    print(x)    -- Should print nil
    x = 50
    print(x)    -- Should print 50
end

print(x)        -- Should print 10

-- Separator
print(123456789)

-- ERROR ON POP
for i = 1, 1 do
    local x
    print(x)    -- Should print nil
    x = 60
    print(x)    -- Should print 60
end

print(x)        -- Should print 10

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
