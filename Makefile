FILES = $(wildcard src/*.cpp)
FLAGS = -Wall -Wextra -Werror -Wpedantic -O3 -std=c++17

FILE_NAME = unv

all: compile strip

compile:
	g++ -o $(FILE_NAME) $(FLAGS) -I./include $(FILES)

strip:
	strip $(FILE_NAME)

install: all
	chmod a+x timedinstall.sh && sudo ./timedinstall.sh

run: all
	./$(FILE_NAME)
