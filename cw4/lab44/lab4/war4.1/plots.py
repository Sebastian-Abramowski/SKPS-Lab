import os
import matplotlib.pyplot as plt

directories = ['server.txt']
def hist():
    files = directories
    all_data = []
    for file in files:
        with open(file, "r") as f:
            data = f.readlines()
        data = [int(line.split(", ")[-1].strip()) for line in data]

        all_data.append(data)
    plt.figure()

    plt.hist(all_data, label=files, bins=20)

    plt.xlabel("Time[usec]")
    plt.ylabel("Signals")
    plt.title("5.2")
    plt.legend()

    plt.savefig("server.png")


def every_sub():
    directories = [d for d in os.listdir() if os.path.isdir(d)]
    for dir in directories:
        hist(dir)


hist()
