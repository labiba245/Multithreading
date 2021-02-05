.default: all

PROGRAMS = main

all: $(PROGRAMS)

clean:
	rm -rf $(PROGRAMS) *.o *.dsYM

main: allocation.o dataGenerator.o main.o
	g++ -Wall -pthread -pedantic -Werror -std=c++17 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -pthread -pedantic -Werror -std=c++17 -g -O -c $^