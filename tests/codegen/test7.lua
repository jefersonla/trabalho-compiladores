function x()
    return 0
end

z = x()

print(z)

function y()
    return 100
end

print(y())

function z(x, y)
    print(x + y)
    return x * y + 100
end

print(z(500, 2))

function t(vaca, boi, ovelha)
    print(vaca)
    print(vaca + boi)
    print(vaca + boi - ovelha)
    local bezero = vaca + boi
    print(bezero)
    return bezero + vaca
end

print(t(10, 20, 5))

print(bezero)

function h(h__, ab__)
    print(h__)
    print(ab__)
    return ab__, h__
end

h2, ab2 = h(40, 80)

print(h2)
print(ab2)