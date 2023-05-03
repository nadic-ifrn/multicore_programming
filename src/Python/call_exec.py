import subprocess

# Path to the C++ binary
cpp_binary_path = '/Users/demetrioscoutinho/Dropbox/IFRN/NADIC/uesc-capacitacao/multicore_programming/Code/OpenCV/gray_scale'

# Parameters to pass to the binary
#param1 = 'hello'
#param2 = 'world'

# Call the binary with the parameters and capture its output
#output = subprocess.check_output([cpp_binary_path, param1, param2])
output = subprocess.check_output([cpp_binary_path])

# Decode the output bytes into a string
output_str = output.decode('utf-8')

# Print the output
print(output_str)