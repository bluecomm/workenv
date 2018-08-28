BINPROG=a.out
CPP_FLAGS := -lm -lrt -lpthread -lprotobuf

all : $BINPROG

$BINPROG : main.o bundler.pb.o
	g++ $(CPP_FLAGS) -o a.out main.o bundler.pb.o

main.o : main.cpp bundler.pb.h
	g++ -c main.cpp

bundler.pb.o : bundler.pb.cc bundler.pb.h
	g++ $(CPP_FLAGS) -c bundler.pb.cc

bundler.pb.cc bundler.pb.h: bundler.proto
	protoc -I=./ --cpp_out=./ bundler.proto

clean :
	rm -rf a.out
	rm -rf *.o
	rm -rf bundler.pb.cc
	rm -rf bundler.pb.h
