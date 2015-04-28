#SHELL:=/bin/bash

# compile *some* libraries as static, others as dynamic: -Wl,-Bstatic ... -Wl,-Bdynamic

CFLAGS:=-Wall -O2 -DNDEBUG
CPPFLAGS:=-I../jsoncpp/include -I/usr/include/OGRE -I/usr/include/OIS
LIB_OGRE:=-lOIS -lOgreMain
LIB_JSON:=-Wl,-Bstatic -L../jsoncpp/libs -ljson -Wl,-Bdynamic

BINARY:=newton-gui newton-calc

newton-gui: basic.o state.o integrator.o simulator.o ogre_framework.o ogre_app.o newton-gui.o
	g++ basic.o state.o integrator.o simulator.o ogre_framework.o ogre_app.o newton-gui.o -o newton-gui ${CFLAGS} ${CPPFLAGS} ${LIB_OGRE} ${LIB_JSON}

newton-calc: basic.o state.o integrator.o simulator.o newton-calc.o
	g++ basic.o state.o integrator.o simulator.o newton-calc.o -o newton-calc ${CFLAGS} ${CPPFLAGS} ${LIB_JSON}

ogre_app.o: ogre_app.cpp ogre_app.h basic.h state.h integrator.h simulator.h ogre_framework.h TextDisplay.h

ogre_framework.o: ogre_framework.cpp ogre_framework.h

simulator.o: simulator.cpp simulator.h basic.h state.h integrator.h
integrator.o: integrator.cpp integrator.h basic.h state.h
state.o: state.cpp state.h basic.h
basic.o: basic.cpp basic.h

%.o: %.cpp
	g++ $< -c -o $@ ${CFLAGS} ${CPPFLAGS}


all: ${BINARY}

debug: ${BINARY}
debug: CFLAGS:=-Wall -g -ggdb

install: ${BINARY}
	cp  ${BINARY} ../
	
clean:
	rm -f *.o
	rm -f ${BINARY}
	rm -f *~
