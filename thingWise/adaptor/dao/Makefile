DIR_INC := ./
DIR_SRC := ./

SRC := $(wildcard ${DIR_SRC}/*.cpp)
OBJ := $(patsubst %.cpp,${DIR_OUT}/%.o,$(notdir ${SRC}))


.PHONY:all clean
all:$(OBJ)

${DIR_OUT}/%.o:${DIR_SRC}/%.cpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@

.PHONY:clean
clean:
	find ${DIR_OUT} -name *.o -exec rm -rf {} \;
