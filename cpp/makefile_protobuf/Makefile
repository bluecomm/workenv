#CPP_FLAGS := -lprotobuf -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Werror -g
CPP_FLAGS := -lprotobuf  -Wall -Werror -g

all : a.out

a.out : base64.o main.o pair.pb.o
	g++ $(CPP_FLAGS) -o a.out base64.o main.o pair.pb.o

main.o : main.cpp pair.pb.h
	g++ $(CPP_FLAGS) -c main.cpp

base64.o : base64.cpp
	g++ $(CPP_FLAGS) -c base64.cpp

pair.pb.o : pair.pb.cc pair.pb.h
	g++ $(CPP_FLAGS) -c pair.pb.cc

pair.pb.cc pair.pb.h: pair.proto
	protoc -I=./ --cpp_out=./ pair.proto --experimental_allow_proto3_optional

clean:
	rm -rf *.o
	rm -rf a.out
	rm -rf *.pb.cc *.pb.h
