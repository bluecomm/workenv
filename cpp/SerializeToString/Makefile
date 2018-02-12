BINPROG=a.out
CPP_FLAGS := -lm -lrt -lpthread -lprotobuf

all : $BINPROG

$BINPROG : main.o base64.o pack.pb.o
	g++ $(CPP_FLAGS) -o a.out main.o base64.o pack.pb.o

main.o : main.cpp pack.pb.h base64.h
	g++ -c main.cpp

base64.o : base64.cpp base64.h
	g++ -c base64.cpp

pack.pb.o : pack.pb.cc pack.pb.h
	g++ $(CPP_FLAGS) -c pack.pb.cc

pack.pb.cc pack.pb.h: pack.proto
	protoc -I=./ --cpp_out=./ pack.proto

clean :
	rm -rf a.out
	rm -rf *.o
	rm -rf pack.pb.cc
	rm -rf pack.pb.h
