.PHONY: usage c mxx web

BUILD := $(shell pwd)/temp
SOURCE := $(shell pwd)

usage:
	echo TODO

c:
	gcc -std=c90 -pedantic-errors -Werror -Wall -Wextra -lglfw -lGL example.c -o temp.example-c

mxx:
	mkdir -p $(BUILD)
	cd $(BUILD) && g++ -std=c++23 -fmodules /usr/include/c++/16.1.1/bits/std.cc -x c++ $(SOURCE)/example.mxx -o app

web:
	node example.mjs
