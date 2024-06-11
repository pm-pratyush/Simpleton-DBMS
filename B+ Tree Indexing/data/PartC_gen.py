import random

# Generate a list of numbers from 1 to 100 in random order
numbers = list(range(1, 101))
random.shuffle(numbers)

# Write the INSERT commands to a file
with open('PartC_input', 'w') as f:
    for num in numbers:
        f.write(f'INSERT {num}\n')

# Perform RANGE operations and write them to the file
with open('PartC_input', 'a') as f:
    for i in range(1, 100, 1):
        f.write(f'RANGE {i} {i+1}\n')

print("Source file 'PartC_inp' generated.")