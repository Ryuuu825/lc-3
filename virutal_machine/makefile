cc := g++
flags := -std=c++14 -Wall -c
targets := main

# Remove the -c flag and link all source files together
main: clean main.cpp
	$(cc) $(flags) -I./include ./lib/*.cpp main.cpp 
	$(cc) -o main *.o

# Remove all object files
clean:
	find . -name "*.o" -type f -delete