CXX    = g++

BIN    = test.exe

OBJS   = test.o

LDLIBS+= -lstdc++

.PHONY:all clean print_all

all:print_all $(BIN)

print_all:
	@echo "----- make all in subdir2 -----"

$(BIN):$(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

%.o:%.cpp
	$(CXX) -c $<

clean:
	 @echo "----- make clean in subdir2 -----"
	rm -f $(BIN) $(OBJS)
