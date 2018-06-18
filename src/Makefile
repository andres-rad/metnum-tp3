CC = g++
CFLAGS = -Wall --std=c++11
DEPS = utils.h matrix.h tomografo.h estructuras.h
OBJ = utils.o matrix.o tomografo.o estructuras.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) main.cpp -o $@ $^ $(CFLAGS)

.PHONY clean:
	rm main *.o
