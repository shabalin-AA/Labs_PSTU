flag = True
while flag==True:
    flag = False
    a = 0
    b = 0
    c = 0
    print("enter a three-digit integer: ")
    n = int(input())
    
    a = n%10
    n = n//10
    b = n%10
    c = n//10

    n = a + b*10 + c*100
    
    if n>999 or n<100:
        flag = True
        print("number must be three-digit integer")
    elif a==b or b==c or a==c:
        flag = True
        print("the numbers should not be repeated")
    else:
        print(a + b*10 + c*100)
        print(a + c*10 + b*100)
        print(b + a*10 + c*100)
        print(b + c*10 + a*100)
        print(c + b*10 + a*100)
        print(c + a*10 + b*100)
        digits = [a, b, c]
        digits.sort()
        digits.reverse()
        max = digits[0]*100+digits[1]*10+digits[2]
        print("max combination is ", max)
        



