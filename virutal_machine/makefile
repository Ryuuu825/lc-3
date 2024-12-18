cc := g++
flags := -std=c++14 -Wall -c
targets := main

# Remove the -c flag and link all source files together
main: clean $(targets).cpp
	$(cc) $(flags) -I./include ./lib/*.cpp $(targets).cpp 
	$(cc) -o $(targets) *.o

# Remove all object files
clean:
	find . -name "*.o" -type f -delete
	rm -f $(targets)