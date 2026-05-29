.PHONY: usage c mxx clean web

BUILDS := $(shell pwd)/temp
SOURCE := $(shell pwd)
IMPORT := /usr/include/c++/16.1.1

C_TARGET := $(BUILDS)/program-c
C_OBJECT := $(BUILDS)/example.o
C_SOURCE := $(SOURCE)/example.c

MXX_TARGET := $(BUILDS)/program-mxx
MXX_OBJECT := $(BUILDS)/example.obj
MXX_SOURCE := $(SOURCE)/example.mxx
MXX_IMPOBJ := $(BUILDS)/std.o
MXX_IMPORT := $(IMPORT)/bits/std.cc

usage:
	@echo TODO

c: $(C_TARGET)

$(C_TARGET): $(C_OBJECT)
	gcc -lglfw -lGL $(C_OBJECT) -o $(C_TARGET)

$(C_OBJECT): $(C_SOURCE)
	@mkdir -p $(BUILDS)
	gcc -std=c90 -pedantic-errors -Werror -Wall -Wextra -c $(C_SOURCE) -o $(C_OBJECT)

mxx: $(MXX_TARGET)

$(MXX_TARGET): $(MXX_OBJECT)
	g++ -lglfw $(MXX_OBJECT) -o $(MXX_TARGET)

$(MXX_OBJECT): $(MXX_SOURCE) $(MXX_IMPOBJ)
	cd $(BUILDS) && g++ -std=c++23 -fmodules -c -x c++ $(MXX_SOURCE) -o $(MXX_OBJECT)

$(MXX_IMPOBJ): $(MXX_IMPORT)
	@mkdir -p $(BUILDS)
	cd $(BUILDS) && g++ -std=c++23 -fmodules -c $(MXX_IMPORT) -o $(MXX_IMPOBJ)

clean:
	@rm -rf temp/

web:
	node example.mjs
