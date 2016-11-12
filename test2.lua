-- System function check_nil check if a given variable is nil or not
-- this is useful to test if type of the variables

-- '>=' and '<=' - OK

print(10 >= 5)          -- 1
print(10 >= 10)         -- 1
print(10 >= 11)         -- 0
print(10 <= 5)          -- 0
print(10 <= 10)         -- 1
print(10 <= 11)         -- 1

-- Separator
print(1111111111)

-- '>' and '<' - OK

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

-- Separator
print(1000000000)

--[[

Operadores logicos (not, and, e or) tem resultado do tipo inteiro, resultando em 0 para falso e 1 para verdadeiro.
A avaliacao das expressoes contendo os operadores and e or devem usar "curto-circuito" (na duvida, veja 
http://ellard.org/dan/www/CS50-95/Notes/mips.html), na qual o segundo operando somente sera' avaliado quando 
necessario. Por exemplo, o operador and deve retornar o  primeiro argumento caso este valor seja 0 ou nil;
caso contrario, devera' retornar o segundo argumento.
O operador or deve retornar o primeiro argumento caso este valor diferente de 0 e de nil; caso contrario, or retorna
o segundo argumento.

]]

-- 'and' and 'or' - OK

print(0 or 1)         -- 1
print(nil and 1)      -- 0
print(not not nil)    -- 0
print(nil or not nil) -- 1
print(5 or 0)         -- 5
print(0 or 6)         -- 6
print(2 and 6)        -- 6
print(0 and 6)        -- 0
print(5 and 1)        -- 1

-- Separator
print(1000000000)

-- Other system functions - OK

print(check_nil(2))         -- 2
print(check_nil(1))         -- 1
print(check_nil(0))         -- 0
print(check_nil(nil))       -- 0
print(check_nil(not nil))   -- 1

-- Separator
print(1111111111)
      
-- Basic comparation operator -- OK

print(nil ~= 0)         -- 1
print(nil == 0)         -- 0
print(nil ~= nil)       -- 0
print(nil == nil)       -- 1

-- Separator
print(1000000000)

-- Basic unary boolean operations -- OK

print(not 1)            -- 0
print(not 50)           -- 0
print(not 0)            -- 1
print(not nil)          -- 1
print(not not nil)      -- 0

-- Separator
print(1212121212)

-- Last tests just for curiosity - OK

print(1 and 2)          -- 2
print(5 or 0)           -- 5
print(nil or 6)         -- 6
print(2 > 1)            -- 1
print(2 <= 0)           -- 0
print(3 == 3)           -- 1
print(1 ~= 1)           -- 0

--a = 2
--print(a)
-- Bugged operators should be treated in semantic --
--print(nil + nil)      -- nil
--print(nil / nil)      -- nil
--print(nil * nil)      -- nil
--print(nil - nil)      -- nil

-- Separator
print(1234567890)

-- Test functions OK
function why()
  while(0) do
    print(3)
  end
  print(11)
  z = 4
  while(z ~= 10) do
      print(2 + z)
      z = z + 1
  end
  for i = 10, 20 do
      print(i)
  end
  for i = 0, 20, 2 do
      print(i)
  end
end

why()

-- Separator
print(1234567890)

while(0) do -- should don't enter here
    print(3)
end

x = 4

-- Test of while, with some assigns 
while(x ~= 0) do -- OK!
    x = x - 1
    print(45)
end

-- Separator
print(1111111111)

-- Test of functions with parameters OK
function kk(xxx, yyy)
    print(xxx)                  -- 100
    print(yyy)                  -- 200
    print(xxx + yyy * 2 / 4)    -- 200
end

xxx = 150

kk(100, 200) -- 100, 200, 200

kk(400, 600) -- 300, 600, 700 