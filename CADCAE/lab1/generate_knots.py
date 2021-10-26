from random import randint

def get_diff_rand():
    return (randint(0, 100)-50) / 1000

knots = 80
tier = 4
weights = [0.9 + get_diff_rand() for _ in range(knots - tier + 1)]

knot = []

for i in range(1, int((knots-1)/(tier-2)+1)):
    for _ in range(tier-2):
        knot.append(i)



# for _ in range(tier-2):
knot.insert(0,1)
knot.append(knots//(tier-2)-1)

# print(knot)
# print(weights)

print(f"splines_comp(10000, {knot}, {weights})")