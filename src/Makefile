CC = g++
CFLAGS = -Wall --std=c++11 -O3
DEPS = utils.h matrix.h tomografo.h estructuras.h csvParser.h generadores_rayos.h defines.h
OBJ = utils.o matrix.o tomografo.o estructuras.o csvParser.o generadores_rayos.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) main.cpp -o $@ $^ $(CFLAGS)

.PHONY clean:
	rm main *.o
