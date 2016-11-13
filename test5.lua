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

-- No more errors on POP - OK
for i = 1, 1 do
    local x
    print(x)    -- Should print nil
    x = 60
    print(x)    -- Should print 60
end

print(x)        -- Should print 10

-- Separator
print(123456789)

-- OK :3
function test()
    local x
    print(x)    -- Should print nil
    x = 30
    print(x)    -- Should print 30
end

test()

print(x)        -- Should print 10

-- Separator
print(123456789)

local k, l, b -- OK

k = 30
print(k)        -- 30
print(l)        -- nil
print(b)        -- nil

-- LOCALS
k, l, b = 10, 20, 30

-- GLOBALS
kk, ll, bb = 10, 20, 30

print(k)        -- 10
print(l)        -- 20
print(b)        -- 30

print(kk)       -- 10
print(ll)       -- 20
print(bb)       -- 30

local zz, zzz = 1, 0

print(zzz)
print(zz)

kkk, kkkk = 2, 4

print(kkk)
print(kkkk)

kkkk, kkk = 4, 8

print(kkk)
print(kkkk)
