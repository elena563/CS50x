import cs50

card = cs50.get_int("Insert your credit card number: ")
sum1 = 0
msum = 0
counter = 0
scard=str(card)
firstdig = int(scard[0])
firsttwo = int(scard[:2])

while card > 0:
    lastdig = card%10
    if counter%2 == 0:
        msum += lastdig
    else:
        doubled = lastdig * 2
        sum1 += (doubled % 10) + (doubled // 10)
    card = card//10
    counter +=1

total = sum1 + msum
if total % 10 == 0:
    if firstdig == 4 and len(scard) in [13, 16]:
        print("VISA")
    elif firsttwo in [34, 37] and len(scard) == 15:
        print("AMEX")
    elif 51 <= firsttwo <= 55 and len(scard) == 16:
        print("MASTERCARD")
    else:
        print("INVALID")
else:
    print("INVALID")
