-- System function check_nil check if a given variable is nil or not
-- this is useful to test if type of the variables

print(10 >= 5)          -- 1
print(10 >= 10)         -- 1
print(10 >= 11)         -- 0
print(10 <= 5)          -- 0
print(10 <= 10)         -- 1
print(10 <= 11)         -- 1

-- Separator
print(1111111111)

-- '>' and '<' - 
print(10 > 5)           -- 1
print(10 > 10)          -- 0
print(10 < 5)           -- 0
print(10 < 11)          -- 1

-- Separator
print(1111111111)

-- '==' and '~=' - OK
print(10 == 11)         -- 0
print(10 == 10)         -- 1
print(10 ~= 10)         -- 0
print(10 ~= -10)        -- 1

-- Separator
print(1111111111)

-- 'not' and '-' - OK
print(-10)              -- -10
print(-(-10))           -- 10
print(not 10)           -- 0
print(not not 1)        -- 1
print(not not not 0)    -- 1

--[[

print(check_nil(2))     -- 2
print(check_nil(1))     -- 1
print(check_nil(0))     -- 0
print(check_nil(nil))   -- 0
print(check_nil(not nil)) -- 1


-- Basic comparation operator --
print(nil == 0)         -- 0
print(nil ~= 0)         -- 1

-- Basic unary boolean operations -- 
print(not 1)            -- 0
print(not 50)           -- 0
print(not 0)            -- 1
print(not nil)          -- 1
print(not not nil)      -- 0
print(1 and 2)          -- 2
print(5 or 0)           -- 5
print(nil or 6)         -- 6
print(2 > 1)            -- 1
print(2 <= 0)           -- 0
print(3 == 3)           -- 1
print(1 ~= 1)           -- 0

-- Bugged operators should be treated in semantic --
print(nil + nil)        -- nil
print(nil / nil)        -- nil
print(nil * nil)        -- nil
print(nil - nil)        -- nil

]]