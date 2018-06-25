INC_DIR:= ./include/ ./include/net/ ./include/threadpool/
SRC_DIR:= src src/net src/threadpool
SRCS_CPP:= $(wildcard src/net/*.cpp) $(wildcard src/threadpool/*.cpp) $(wildcard src/*.cpp)
OCC := $(patsubst %.cc, %.o, $(wildcard src/*.cc)) 
OCPP := $(patsubst %.cpp, %.o, $(SRCS_CPP))
OBJS:= $(OCC) $(OCPP)
LIBS:= -lpthread -llog4cpp -lhiredis -ljson

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/main

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf ./src/*.o ./src/net/*.o ./src/threadpool/*.o
