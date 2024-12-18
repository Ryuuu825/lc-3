cc := g++
flags := -std=c++14 -Wall -c
targets := lc3ac

# Remove the -c flag and link all source files together
main: clean $(targets).cpp
	$(cc) $(flags) -I./include ./lib/*.cpp $(targets).cpp 
	$(cc) -o $(targets) *.o
	./$(targets)
	../virutal_machine/main

# Remove all object files
clean:
	find . -name "*.o" -type f -delete
	rm -f $(targets)
	rm -f test.bin