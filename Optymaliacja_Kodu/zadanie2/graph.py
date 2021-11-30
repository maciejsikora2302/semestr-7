import matplotlib.pyplot as plt

my_opt_name = []
my_opt_val = []

o3_name = []
o3_val = []


with open("./wyniki.txt", "r") as wyniki:
    for i, line in enumerate(wyniki.readlines()):
        if i == 4: continue
        splited = line.split(",")
        val = float(splited[0])
        name = str(splited[1])
        print(f"val: {val}, name: {name}", end='')
        if i < 4:
            my_opt_val.append(val)
            my_opt_name.append(name)
        else:
            o3_val.append(val)
            o3_name.append(name)

plt.plot(my_opt_name, my_opt_val, label = "Moje optymalizacje")
plt.plot(o3_name, o3_val, label = "Flaga -O3")
plt.title("Porównanie ręcznych optymalizacji z wykorzystaniem flagi -O3")
plt.legend(loc="upper right")
plt.ylabel("Sekundy")
plt.show()