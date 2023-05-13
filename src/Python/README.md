# 🚀 Chamando C a partir de Python usando ctypes 🐍

1. **Compilação do Código C** 

    Antes de tudo, você precisará compilar o código C em uma biblioteca compartilhada que possa ser carregada em Python. 

    ```bash
    gcc -shared -o libestimatepi.so estimatepi.c -fopenmp
    ```

    Isso criará um arquivo de biblioteca compartilhada chamado `libestimatepi.so` que pode ser carregado em Python usando `ctypes`.

2. **Carregando a Biblioteca Compartilhada em Python**

    Agora que temos nossa biblioteca compartilhada, podemos carregá-la em Python. O código a seguir mostra como isso pode ser feito:

    ```python
    import ctypes

    # Carregue a biblioteca compartilhada
    lib = ctypes.CDLL('./libestimatepi.so')
    ```

    Este código Python carrega a biblioteca compartilhada `libestimatepi.so`.

3. **Configurando os Tipos de Argumentos e Retorno**

    Em seguida, especificamos o tipo de argumento e o tipo de retorno da função `calculate_pi`:

    ```python
    # Especifique o tipo de argumento e o tipo de retorno da função calculate_pi
    lib.calculate_pi.argtypes = [ctypes.c_ulong]
    lib.calculate_pi.restype = ctypes.c_double
    ```

    Agora, nossa função `calculate_pi` está pronta para ser chamada de Python.

4. **Chamando a Função C a partir de Python**

    Finalmente, podemos chamar a função `calculate_pi` de Python. Para isso, utilizamos o seguinte código:

    ```python
    # Agora você pode chamar a função calculate_pi de Python
    n = 100000000
    pi = lib.calculate_pi(n)

    print("Pi: ", pi)
    ```

    Este código chama a função `calculate_pi` com o argumento `n` e imprime o resultado.

    Nota: As bibliotecas devem estar no mesmo diretório do arquivo python ou no caminho de busca de bibliotecas do sistema para serem encontradas pelo `ctypes.CDLL`.

5. **Executando o Código Python**

Certifique-se de estar no mesmo diretório que a biblioteca compartilhada `libestimatepi.so` e execute o script Python. Você deve ver o valor calculado de Pi impresso na saída.

## Observações

Note que isso irá executar o código C exatamente como é. Se você quiser passar argumentos para o código C a partir do Python, você terá que passar esses argumentos para a função principal, e o código C terá que ser escrito de maneira a aceitar esses argumentos.

Adicionalmente, para interações mais complexas entre Python e C/C++, pode ser utilizado bibliotecas mais robustas como `Cython`, `CFFI` ou `SWIG`.
