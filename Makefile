CC = g++

board.o : board.cpp board.hpp
	$(CC) -std=c++2a -c board.cpp

game.o : game.cpp game.hpp
	$(CC) -std=c++2a -c game.cpp -lstdc++

Reversi : main.cpp game.o board.o
	$(CC) -std=c++2a game.o board.o main.cpp -o Reversi.exe -lstdc++

ml.o : machine_learning.cpp machine_learning.hpp
	$(CC) -std=c++2a -c machine_learning.cpp -o ml.o -lstdc++

MachineLearning : learn.cpp board.o ml.o
	$(CC) -std=c++2a board.o ml.o learn.cpp -o MachineLearning.exe -lstdc++ -fopenmp

all : clean Reversi

clean:
	-rm -f *.o *.exe
