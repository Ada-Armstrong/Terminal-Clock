CXX = gcc
CXXFLAGS = -g -Wall -MMD -Werror=vla
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

OBJECTS = bresenham_line.o main.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = stc

########## Targets ##########

.PHONY : clean

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -lm -lncurses -o $@

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean :
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}

