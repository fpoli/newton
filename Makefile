#SHELL:=/bin/bash

BINARY:= newton-gui newton-calc

CXX ?= g++

all:
	cd src && make all

debug:
	cd src && make debug

install:
	cd src && make install

check:
	@# TODO: fix more suppressed messages

	@echo "=== CppLint ==="
	@cpplint --quiet --linelength=200 \
		--filter=-whitespace,-legal/copyright,-build/include_subdir,\
-build/header_guard,-build/include_order,-build/namespaces,-runtime/explicit,\
-runtime/references,-runtime/indentation_namespace,-readability/braces,\
-readability/casting,-readability/namespace,-readability/todo \
		src/*.h src/*.cpp src/ogre/*.h

	@echo "=== CppCheck ==="
	@cppcheck --quiet --enable=all \
		--suppress=unusedFunction --suppress=noCopyConstructor \
		--suppress=missingInclude --suppress=missingIncludeSystem \
		src/

test:
	@for cfg in scripts/*.conf; do \
		echo "=== Test configuration $$cfg ==="; \
		./newton-calc "$$cfg" 3600 "$$cfg.out"; \
	done

clean:
	rm -f *~
	rm -f ${BINARY}
	cd src && make clean

sandwich:
	@if [ `/usr/bin/id -u` -ne 0 ]; then echo "What? Make it yourself."; else echo "Okay."; fi
	@echo "http://xkcd.com/149/"

wargames:
	@echo "Would you like to play a game?"
	@read foo
	@echo "A strange game."; echo "The only winning move is"; echo "not to play."
