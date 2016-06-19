#SHELL:=/bin/bash

SUBDIRS:= src
BINARY:= newton-gui newton-calc

all:
	@for dir in $(SUBDIRS) ; do \
		cd $$dir ; make all ; cd .. ; \
	done;

debug:
	@for dir in $(SUBDIRS) ; do \
		cd $$dir ; make debug ; cd .. ; \
	done ;

install:
	@for dir in $(SUBDIRS) ; do \
		cd $$dir ; make install ; cd .. ; \
	done ;

lib:
	sudo apt-get install build-essential libjsoncpp-dev libois-dev \
		libogre-1.9-dev

clean:
	rm -f *~
	rm -f ${BINARY}
	@for dir in $(SUBDIRS) ; do \
		cd $$dir ; make clean ; cd .. ; \
	done ;

sandwich:
	@if [ `/usr/bin/id -u` -ne 0 ]; then echo "What? Make it yourself."; else echo "Okay."; fi
	@echo "http://xkcd.com/149/"

wargames:
	@echo "Would you like to play a game?"
	@read foo
	@echo "A strange game."; echo "The only winning move is"; echo "not to play."
