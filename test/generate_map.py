import random

def generate_matrix_file(n, m, max_value, file_name):
    """
    Generate an n x m matrix with random values up to max_value and save to a file.
    
    :param n: Number of rows
    :param m: Number of columns
    :param max_value: Maximum value for each cell
    :param file_name: Name of the file to save the matrix
    """
    with open(file_name, 'w') as f:
        for _ in range(n):
            row = ' '.join(str(random.randint(1, max_value)) for _ in range(m))
            f.write(row + '\n')

# Taking inputs from the user
n = int(input("Enter the number of rows (n): "))
m = int(input("Enter the number of columns (m): "))
max_value = int(input("Enter the maximum value for each cell: "))
file_name = input("Enter the name of the file to save the matrix: ")

# Generating the file in the current directory
generate_matrix_file(n, m, max_value, file_name)
print(f"Generated a {n}x{m} matrix with numbers up to {max_value} in '{file_name}'.")
