-- Testing
x, y, z = 1, 2, 3
a = 2 + 3

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