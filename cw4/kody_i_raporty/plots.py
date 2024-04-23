import os
import matplotlib.pyplot as plt


def hist(directory):
    files = [file for file in os.listdir(directory) if (file[:3] == "cli")]
    all_data = []
    for file in files:
        with open(directory + "\\" + file, "r") as f:
            data = f.readlines()
        data = [int(line.split(", ")[-1].strip()) for line in data]

        all_data.append(data)
    plt.figure()

    plt.hist(all_data, label=files, bins=20)

    plt.xlabel("Time[usec]")
    plt.ylabel("Signals")
    plt.title("title")
    plt.legend()

    plt.savefig(directory + "\\hist2.png")


def every_sub():
    directories = [d for d in os.listdir() if os.path.isdir(d)]
    for dir in directories:
        hist(dir)


every_sub()
