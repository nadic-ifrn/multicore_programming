import ctypes

# Carregue a biblioteca compartilhada
lib = ctypes.CDLL('./libestimatepi.so')

# Especifique o tipo de argumento e o tipo de retorno da função calculate_pi
lib.calculate_pi.argtypes = [ctypes.c_ulong]
lib.calculate_pi.restype = ctypes.c_double

# Agora você pode chamar a função calculate_pi de Python
n = 100000000
pi = lib.calculate_pi(n)

print("Pi: ", pi)
