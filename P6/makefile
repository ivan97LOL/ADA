# Iván Álvarez García DNI: 49623492A
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze 

maze: maze.cc
	g++ ${OPTS} -o maze maze.cc	 

tar: maze.cc makefile
	tar cvfz 49623492A.tar.gz maze.cc makefile

clean: 
	rm -f pow2 pow2.Steps fit.log pow2*.png *~
 
