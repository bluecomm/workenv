CC=g++
#CFLAGS=-Wall -Werror -g -lprotobuf -std=c++11
#CFLAGS=-Wall -Werror -g -lprotobuf -D_GLIBCXX_USE_CXX11_ABI=0
CFLAGS=-Wall -Werror -g -lprotobuf
CPP_FLAGS := -lm -lrt -lpthread -lprotobuf
LD=g++
LDFLAGS=$(LIBS)
DEPENDFLAG=-MM
LIBS=

CFLAGS:=$(CFLAGS) $(INCL)
LDFLAGS:=$(LDFLAGS) $(LIBS)

RMF=-rm -f
SRCDIRS=.
INCDIRS=.
PROGRAMS=a.out

INCL=$(addprefix -I,$(INCDIRS))

###############################################################
SRCS_CPP=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))
SRCS_CC=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.pb.cc))
SRCS_PB=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.proto))

OBJS_CPP=$(SRCS_CPP:.cpp=.o)
OBJS_CC=$(SRCS_CC:.pb.cc=.pb.o)
OBJS_PB=$(SRCS_PB:.proto=.pb.cc)
OBJS=$(OBJS_CPP) $(OBJS_CC)

all:$(PROGRAMS)

$(PROGRAMS):$(OBJS) $(OBJS_PB)
	$(CC) $(CFLAGS) $(CPP_FLAGS) -o $@ $(filter %.o ,$+)
#	$(CC) $(LDFLAGS) $(CFLAGS) $(CPP_FLAGS) -o $@ $(filter %.o ,$+)

%.o:%.cpp
	$(CC) $(INCL) $(CFLAGS) $(CPP_FLAGS) -DDEBUG -o $@ -c $<

%.pb.o:%.pb.cc
	$(CC) $(INCL) $(CFLAGS) $(CPP_FLAGS) -DDEBUG -o $@ -c $<

#%.pb.cc:%.proto
PB:
	protoc -I=./ --cpp_out=./ *.proto

clean:
	$(RMF) $(OBJS) $(DEPENDS_CPP) $(DEPENDS_CC) $(PROGRAMS) *.pb.cc *.pb.h

info:
	@echo 'OBJS_CPP: ' $(OBJS_CPP)
	@echo 'OBJS_CC: ' $(OBJS_CC)
	@echo 'OBJS_PB: ' $(OBJS_PB)
	@echo 'OBJS: ' $(OBJS)
	@echo 'SRCS_CPP: ' $(SRCS_CPP)
	@echo 'SRCS_CC: ' $(SRCS_CC)
	@echo 'SRCS_PB: ' $(SRCS_PB)
