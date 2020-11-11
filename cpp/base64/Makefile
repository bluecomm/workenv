#CPP_FLAGS := -lboost
CPP_FLAGS := 

all : a.out

a.out : base64.o main.o
	g++ $(CPP_FLAGS) -o a.out base64.o main.o

main.o : main.cpp
	g++ $(CPP_FLAGS) -c main.cpp

base64.o : base64.cpp
	g++ $(CPP_FLAGS) -c base64.cpp

clean:
	rm -rf *.o
	rm -rf a.out
