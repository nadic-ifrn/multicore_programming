# Exemplos com OpenMP :file_folder:

Aqui estão alguns dos programas que utilizamos para ilustrar o uso do OpenMP:

- `hello.cpp` - Velho e bom Hello world.
- `sections.cpp` - Exemplificando o uso de seções.
- `scope.cpp` - Exemplificando o escopo de variáveis.
- `schedules` - exemplificando o uso de schedules.
- `estimatepi` - Método para estimar o número PI.
- `histogram` - Algoritmo simples para calcular um histograma.
- `cgm` - Implementação do Método do Gradiente Conjugado.

# Compilação :hammer:

Na maioria das vezes, os códigos podem ser compilados da mesma maneira. No entanto, alguns códigos podem necessitar de flags adicionais. Verifique a primeira linha de cada arquivo para confirmar a linha de comando de compilação.

- Para Mac: 
```bash
clang -fopenmp nome_do_arquivo.c -o nome_do_arquivo
clang++ -fopenmp nome_do_arquivo.cpp -o nome_do_arquivo
```
- Para Linux: 
```bash
gcc -fopenmp nome_do_arquivo.c -o nome_do_arquivo
g++ -fopenmp nome_do_arquivo.cpp -o nome_do_arquivo
```