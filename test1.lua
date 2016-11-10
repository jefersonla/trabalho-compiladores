-- Testing
--x, y, z = 1, 2, 3
--a = 2 + 3
    
print(4 / 2)    -- 2
print(7)        -- 7
print(2 + 1)    -- 3
print(2 * 2)    -- 4
print(not 1)    -- 0
print(not 0)    -- 1
print(nil)      -- nil
print(3+40/2)   -- 23
print(1 or 0)   -- 1
print(1 and 0)  -- 0
print(5>3)      -- 1
print(4<2)      -- 0
print(5>=8)     -- 0
print(8<=10)    -- 1
print(2 / 4)    -- SHOULD BE 0
print(not nil)                  -- 1 
print(not not nil)              -- 0
print(0 and 0 and 1)            -- 0
print((1 + 3) + (2 * (3+1)))    -- 12

function x()
    print(888)
end

x() -- 888 OK

--[[

# tendo a variavel em a0
# eu pego ela 
# carego a mascara em $t1
# faço xor de $a0 e $t1 e jogo em $t0
# nego $t0
# se $t0 igual a $0 é nil

# Checa nil
li $t1, 0x80000000
xor $t0, $a0, $t1
not $t0, $t0
beq $t0, $0, nil_number

]]
