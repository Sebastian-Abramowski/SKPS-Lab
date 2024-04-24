import os
import matplotlib.pyplot as plt
import numpy as np
import copy


def hist():
    files = [file for file in os.listdir() if (file[:3] == "ser")]
    all_data = []
    for file in files:
        with open(file, "r") as f:
            data = f.readlines()
        data = [int(line.split(", ")[-1].strip()) for line in data[1:]]
        costam = np.average(data)
        data_copy = copy.deepcopy(data)
        data = [min(i, costam) for i in data_copy]
        all_data.append(data)
    plt.figure()
    plt.hist(all_data, label=files, bins=20)

    plt.xlabel("Time[usec]")
    plt.ylabel("Signals")
    plt.title("zad6")
    plt.legend()

    plt.savefig(f"{'zad6'}_hist.png")


def every_sub():
    directories = [d for d in os.listdir() if os.path.isdir(d)]
    for dir in directories:
        hist(dir)


hist()
