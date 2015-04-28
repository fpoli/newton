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
	# Assicurati di poter compilare (g++, ecc)
	sudo apt-get install build-essential mercurial cmake-gui

	# Installa jsoncpp (parser json)
	sudo apt-get install libjsoncpp-dev

	# Insalla la libreria OIS (per input da tastiera e mouse) richiesta da OGRE
	sudo apt-get install libois-dev

	# Installa OGRE (motore grafico 3D)
	hg clone https://bitbucket.org/sinbad/ogre -r v1-9-0 ogre
	sudo apt-get install libfreetype6-dev libboost-date-time-dev \
		libboost-thread-dev nvidia-cg-toolkit libfreeimage-dev \
		zlib1g-dev libzzip-dev libois-dev libcppunit-dev doxygen \
		libxt-dev libxaw7-dev libxxf86vm-dev libxrandr-dev libglu-dev
	cd ogre && cmake . && make -j 4 && sudo make install && sudo ldconfig

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
