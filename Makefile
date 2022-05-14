CC = g++

board.o : board.cpp board.hpp
	$(CC) -std=c++11 -c board.cpp

game.o : game.cpp game.hpp
	$(CC) -std=c++11 -c game.cpp -lstdc++

Reversi : main.cpp game.o board.o
	$(CC) -std=c++11 game.o board.o main.cpp -o Reversi.exe -lstdc++

all : clean Reversi

clean:
	-rm -f *.o
