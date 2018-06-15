INC_DIR:= ./include/
SRC_DIR:= ./src
SRCS:=$(wildcard ./src/*.cc) 
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -llog4cpp -lpthread -lboost_regex  

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/main

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
