BINPROG=a.out
CPP_FLAGS := -lm -lrt -lpthread -lprotobuf

all : $BINPROG

$BINPROG : main.o test_field.pb.o
	g++ $(CPP_FLAGS) -o a.out main.o test_field.pb.o

main.o : main.cpp test_field.pb.h
	g++ -c main.cpp
#	g++ -c main.cpp test_field.pb.h

test_field.pb.o : test_field.pb.cc test_field.pb.h
	g++ -c test_field.pb.cc
#	g++ -c test_field.pb.cc test_field.pb.h

test_field.pb.cc test_field.pb.h: test_field.proto
	protoc -I=./ --cpp_out=./ test_field.proto

clean :
	rm -rf a.out
	rm -rf *.o
	rm -rf test_field.pb.cc
	rm -rf test_field.pb.h
