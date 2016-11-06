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
print(nil and 1)      -- 0 -- nil pelo professor .... 0
print(not not nil)    -- 0 -- nil, ta certo ... 0
print(nil or not nil) -- 1 -- not nil... eu acho, eu penso ... CRENDEUSPAI SEI O QUE É ISSO AQUI NÃO, PELA CARA È MACUMBA

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

print(1 and 2)          -- 1
print(5 or 0)           -- 1
print(nil or 6)         -- 1
print(2 > 1)            -- 1
print(2 <= 0)           -- 0
print(3 == 3)           -- 1
print(1 ~= 1)           -- 0

-- Bugged operators should be treated in semantic --
--print(nil + nil)      -- nil
--print(nil / nil)      -- nil
--print(nil * nil)      -- nil
--print(nil - nil)      -- nil