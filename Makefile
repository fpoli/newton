#SHELL:=/bin/bash

SUBDIRS = src

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
	sudo apt-get install build-essential
	
	# Scarica e compila jsoncpp (parser json)
	sudo apt-get install scons
	# non andare a capo!
	if [ ! -d "jsoncpp" ]; \
	then \
		wget "http://downloads.sourceforge.net/project/jsoncpp/jsoncpp/0.5.0/jsoncpp-src-0.5.0.tar.gz"; \
		tar -xvzf jsoncpp-src-0.5.0.tar.gz; \
		mv jsoncpp-src-0.5.0 jsoncpp; \
	fi
	# non andare a capo!
	cd jsoncpp ; scons platform=linux-gcc; cp libs/*/*.a libs/libjson.a; cd ..;
	
	# Insalla la libreria OIS (per input da tastiera e mouse) richiesta da OGRE
	sudo apt-get install libois-dev
	
	# Installa OGRE (motore grafico 3D)
	sudo add-apt-repository ppa:ogre-team/ogre
	sudo apt-get update
	sudo apt-get install libogre-dev ogre-samples-media
	
clean:
	rm -f *~
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
