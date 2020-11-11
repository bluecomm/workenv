#CPP_FLAGS := -lprotobuf -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Werror -g
CPP_FLAGS := -lprotobuf  -Wall -Werror -g

all : a.out

a.out : main.o test.pb.o
	g++ $(CPP_FLAGS) -o a.out main.o test.pb.o

main.o : main.cpp test.pb.h
	g++ $(CPP_FLAGS) -c main.cpp

test.pb.o : test.pb.cc test.pb.h
	g++ $(CPP_FLAGS) -c test.pb.cc

test.pb.cc test.pb.h: test.proto
	protoc -I=./ --cpp_out=./ test.proto

clean:
	rm -rf *.o
	rm -rf a.out
	rm -rf *.pb.cc *.pb.h
