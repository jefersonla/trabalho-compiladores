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