#SHELL:=/bin/bash

OGRE_INCLUDE:=/usr/local/include/OGRE

# compile *some* libraries as static, others as dynamic: -Wl,-Bstatic ... -Wl,-Bdynamic
CFLAGS:=-Wall -O2 -DNDEBUG -Wno-overloaded-virtual
CPPFLAGS:=-I${OGRE_INCLUDE} -I${OGRE_INCLUDE}/Overlay \
	-I/usr/include/OIS -I/usr/include/jsoncpp
LIBS:=-lpthread -lm -Wl,-Bstatic -ljsoncpp -Wl,-Bdynamic
LIB_OGRE:=-lOIS -lOgreMain -lOgreOverlay -lboost_system

BINARY:=newton-gui newton-calc

CXX ?= g++

newton-gui: basic.o state.o integrator.o simulator.o ogre_framework.o ogre_app.o newton-gui.o
	$(CXX) basic.o state.o integrator.o simulator.o \
		ogre_framework.o ogre_app.o newton-gui.o \
		-o newton-gui ${CFLAGS} ${CPPFLAGS} ${LIBS} ${LIB_OGRE}

newton-calc: basic.o state.o integrator.o simulator.o newton-calc.o
	$(CXX) basic.o state.o integrator.o simulator.o newton-calc.o \
		-o newton-calc ${CFLAGS} ${CPPFLAGS} ${LIBS}

ogre_app.o: ogre_app.cpp ogre_app.h basic.h state.h integrator.h simulator.h ogre_framework.h TextDisplay.h

ogre_framework.o: ogre_framework.cpp ogre_framework.h

simulator.o: simulator.cpp simulator.h basic.h state.h integrator.h
integrator.o: integrator.cpp integrator.h basic.h state.h
state.o: state.cpp state.h basic.h
basic.o: basic.cpp basic.h

%.o: %.cpp
	$(CXX) $< -c -o $@ ${CFLAGS} ${CPPFLAGS}


all: ${BINARY}

debug: ${BINARY}
debug: CFLAGS:= -g -ggdb -Wall -Wextra -Wformat -Wformat-security \
	            -Wno-unused-parameter -Wno-unused-but-set-parameter \
	            -Wno-overloaded-virtual

install: ${BINARY}
	cp  ${BINARY} ../

clean:
	rm -f *.o
	rm -f ${BINARY}
	rm -f *~
