#SHELL:=/bin/bash

CXX ?= g++
CFLAGS:=-Wall -O2 -DNDEBUG -Wno-overloaded-virtual
CPPFLAGS:=$(shell pkg-config --cflags jsoncpp OIS OGRE OGRE-Overlay)
LIBS:=$(shell pkg-config --libs jsoncpp OIS OGRE OGRE-Overlay) -lboost_system

BINARY:=newton-gui newton-calc


newton-gui: basic.o state.o integrator.o simulator.o ogre_framework.o ogre_app.o newton-gui.o
	$(CXX) basic.o state.o integrator.o simulator.o \
		ogre_framework.o ogre_app.o newton-gui.o \
		-o newton-gui $(CFLAGS) $(CPPFLAGS) $(LIBS)

newton-calc: basic.o state.o integrator.o simulator.o newton-calc.o
	$(CXX) basic.o state.o integrator.o simulator.o newton-calc.o \
		-o newton-calc $(CFLAGS) $(CPPFLAGS) $(LIBS)

ogre_app.o: ogre_app.cpp ogre_app.h basic.h state.h integrator.h simulator.h ogre_framework.h TextDisplay.h

ogre_framework.o: ogre_framework.cpp ogre_framework.h

simulator.o: simulator.cpp simulator.h basic.h state.h integrator.h
integrator.o: integrator.cpp integrator.h basic.h state.h
state.o: state.cpp state.h basic.h
basic.o: basic.cpp basic.h

%.o: %.cpp
	$(CXX) $< -c -o $@ $(CFLAGS) $(CPPFLAGS)

all: $(BINARY)

debug: $(BINARY)
debug: CFLAGS := -g -ggdb -Wall -Wextra -Wformat -Wformat-security \
				 -Wno-unused-parameter -Wno-unused-but-set-parameter \
				 -Wno-overloaded-virtual

clean:
	rm -f *.o
	rm -f $(BINARY)
	rm -f *~
