# Plot the distribution of block accesses needed with and without the B+Tree when calling RANGE i i+1 on every valid i as two seperate traces (with and without)  and compare the results.

import matplotlib.pyplot as plt
import numpy as np
import sys

if __name__ == "__main__":
    # Store the command line arguments
    args = sys.argv
    if len(args) != 3:
        print("Usage: python3 PartC_Plot.py <filename> <fanout>")
        exit(1)
    
    filename = args[1]
    fanout = int(args[2])

    # Make 2 dictionaries to store the number of block accesses
    with_bptree = {}
    without_bptree = {}

    # Read the file
    with open(filename, 'r') as f:
        for line in f:
            inp = line.split()

            if inp[0] not in with_bptree.keys():
                with_bptree[inp[0]] = 0
            with_bptree[inp[0]] += 1

            if inp[1] not in without_bptree.keys():
                without_bptree[inp[1]] = 0
            without_bptree[inp[1]] += 1
        
        # Sort the dictionaries
        sorted_with_bptree = sorted(with_bptree.items())
        sorted_without_bptree = sorted(without_bptree.items())

        # Convert the dictionaries
        dict_with_bptree = dict(sorted_with_bptree)
        dict_without_bptree = dict(sorted_without_bptree)

        # Plot the graph
        fig, ax = plt.subplots()
        ax.set_title(f"Block Accesses for RANGE i - i+1 with fanout {fanout}")
        ax.set_xlabel("Number of Block Accesses")
        ax.set_ylabel("Frequency")

        ax.bar(dict_with_bptree.keys(), dict_with_bptree.values(), width=0.5, label="With B+Tree")
        ax.bar(dict_without_bptree.keys(), dict_without_bptree.values(), width=0.5, label="Without B+Tree")

        ax.legend()
        plt.show()