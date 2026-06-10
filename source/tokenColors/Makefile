.PHONY: usage c run-c mxx run-mxx clean run-web gen-ccjson
# variables
BUILDS := $(shell pwd)/build
SOURCE := $(shell pwd)
IMPORT := /usr/include/c++/16.1.1
# c variables
C_TARGET := $(BUILDS)/program-c
C_OBJECT := $(BUILDS)/example.o
C_SOURCE := $(SOURCE)/example.c
# mxx variables
MXX_TARGET := $(BUILDS)/program-mxx
MXX_OBJECT := $(BUILDS)/example.obj
MXX_SOURCE := $(SOURCE)/example.mxx
MXX_IMPOBJ := $(BUILDS)/std.o
MXX_IMPORT := $(IMPORT)/bits/std.cc
# web variables
WEB_SOURCE := $(SOURCE)/example.mjs
# generate compile commands variables
GEN_CCJSON := $(BUILDS)/compile_commands.json
# usage
usage:
	@echo "usage: make [command]"
	@echo -e "\t[command] :"
	@echo -e "\tgen-ccjson: generate compile commands json"
	@echo -e "\tc         : compile c example"
	@echo -e "\trun-c     : run c example"
	@echo -e "\tmxx       : compile mxx example"
	@echo -e "\trun-mxx   : run mxx example"
	@echo -e "\tclean     : remove build directory"
	@echo -e "\trun-web   : run nodejs example"
# fgf
gen-ccjson:
	@mkdir -p $(BUILDS)
	@echo -e "[{"                                            > $(GEN_CCJSON)
	@echo -e '\t"directory": "$(SOURCE)",'                  >> $(GEN_CCJSON)
	@echo -e '\t"command": "clang -std=c90 -c -x c",'       >> $(GEN_CCJSON)
	@echo -e '\t"file": "$(C_SOURCE)"'                      >> $(GEN_CCJSON)
	@echo -e '}, {'                                         >> $(GEN_CCJSON)
	@echo -e '\t"directory": "$(SOURCE)",'                  >> $(GEN_CCJSON)
	@echo -e '\t"command": "clang++ -std=c++17 -c -x c++",' >> $(GEN_CCJSON)
	@echo -e '\t"file": "$(CXX_SOURCE)"'                    >> $(GEN_CCJSON)
	@echo -e '}, {'                                         >> $(GEN_CCJSON)
	@echo -e '\t"directory": "$(SOURCE)",'                  >> $(GEN_CCJSON)
	@echo -e '\t"command": "clang++ -std=c++23 -c -x c++",' >> $(GEN_CCJSON)
	@echo -e '\t"file": "$(MXX_SOURCE)"'                    >> $(GEN_CCJSON)
	@echo -e "}]"                                           >> $(GEN_CCJSON)
# c example related targets
# -- run
run-c: $(C_TARGET)
	$(C_TARGET)
# -- command
c: $(C_TARGET)
# -- target
$(C_TARGET): $(C_OBJECT)
	gcc -lglfw -lGL $(C_OBJECT) -o $(C_TARGET)
# -- object
$(C_OBJECT): $(C_SOURCE)
	@mkdir -p $(BUILDS)
	gcc -std=c90 -pedantic-errors -Werror -Wall -Wextra -c $(C_SOURCE) -o $(C_OBJECT)
# mxx example related targets
# -- run
run-mxx: $(MXX_TARGET)
	$(MXX_TARGET)
# -- command
mxx: $(MXX_TARGET)
# -- link executable
$(MXX_TARGET): $(MXX_OBJECT)
	g++ -lSDL3 -lvulkan $(MXX_OBJECT) -o $(MXX_TARGET)
# -- compile object file
$(MXX_OBJECT): $(MXX_SOURCE) $(MXX_IMPOBJ)
	cd $(BUILDS) && g++ -std=c++23 -fmodules -DAPP_IMPORT_STD -c -x c++ $(MXX_SOURCE) -o $(MXX_OBJECT)
# -- import std for modules
$(MXX_IMPOBJ): $(MXX_IMPORT)
	@mkdir -p $(BUILDS)
	cd $(BUILDS) && g++ -std=c++23 -fmodules -c $(MXX_IMPORT) -o $(MXX_IMPOBJ)
# remove build artifacts
clean:
	@rm -rf $(BUILDS)
	@git restore $(BUILDS)/.gitignore
# web related targets
run-web:
	node $(WEB_SOURCE)
