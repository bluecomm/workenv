BIN=sha512

all:$(BIN)

$(BIN):main.o sha512.o
	g++ -o $(BIN) main.o sha512.o

main.o:main.cpp sha512.h
	g++ -I. -c main.cpp

sha512.o:sha512.cpp sha512.h
	g++ -I. -c sha512.cpp

clean:
	rm main.o sha512.o
	rm $(BIN)
