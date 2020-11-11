ver=release
CXX=g++
ifeq ($(ver),debug)
CXXFLAGS=-std=c++11 -Wall -Wextra -g -O0 -DDEBUG
else
CXXFLAGS=-std=c++11 -Wall
#-Wextra
endif
BIN=project 
ALLDIR=$(shell find . -type d | grep -E '/\.|proto|test' -v)
SRCS=$(foreach DIR,$(ALLDIR), $(wildcard $(DIR)/*.cpp))
OBJS=$(patsubst %.cpp,%.o, $(SRCS))
PROTODIR=$(shell find . -type d | grep -E '/proto$$')
PROTOFILE=$(wildcard $(PROTODIR)/*.proto)
PROTOSRC=$(patsubst %.proto,%.pb.cc,$(PROTOFILE)) $(patsubst %.proto,%.grpc.pb.cc,$(PROTOFILE))
PROTOHEAD=$(patsubst %.cc,%.h,$(PROTOSRC))
PROTOOBJS=$(patsubst %.cc,%.o, $(PROTOSRC)) 

ALL: $(PROTOOBJS) $(OBJS)
    $(CXX) $(CXXFLAGS) -o $(BIN) $^ -pthread -lprotobuf -lgrpc++ %.o: %cc %.cpp $(CXX) $(CXXFLAGS) -o $@ -c $< 

$(PROTOSRC): $(PROTOFILE)
	cd $(PROTODIR);protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $(notdir $(PROTOFILE))
	cd $(PROTODIR);protoc --cpp_out=. $(notdir $(PROTOFILE))

clean:
	rm -f $(OBJS) $(PROTOOBJS) $(BIN)
