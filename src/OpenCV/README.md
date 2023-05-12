# Exemplos com OpenMP e OpenCV :file_folder:

Confira alguns dos programas que utilizamos para demonstrar a aplicação do OpenMP e OpenCV:

- `gray_scale`: Converte uma imagem para a escala de cinza.
- `warp-1.0`: Aplica um filtro de distorção na imagem.
- `dicom_2d_volume`: Exemplo simples que demonstra como unificar uma sequência de slices.

# Compilação :hammer:

Normalmente, os códigos podem ser compilados da mesma maneira. No entanto, alguns códigos podem necessitar de flags adicionais. Recomendamos verificar a primeira linha de cada arquivo para confirmar a linha de comando de compilação.

- Para Mac: 
```bash
clang++ -fopenmp -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs nome_do_arquivo.cpp -o bin/nome_do_arquivo
```
- Para Linux: 
```bash
g++ nome_do_arquivo.cpp -o nome_do_arquivo `pkg-config --cflags --libs opencv`
```