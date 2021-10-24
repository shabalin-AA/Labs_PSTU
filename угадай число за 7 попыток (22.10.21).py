import random
number = random.randint(1,100)
print("you need to guess the number from 1 to 100")
for i in range(8):
    if i!=7:
        print("attempt ", i+1)
        
        n = int(input())

        if n>number:
            print("your number is greater")
        elif n<number:
            print("your number is less")
        else:
            print("you won")
            break
    else:
        print("you lose")
