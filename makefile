LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: toweroffense

main.o: main.cc
	g++ -c main.cc -I SFML-2.4.2/include/ -std=c++11

TextureManager.o: TextureManager.cc
	g++ -c TextureManager.cc -I SFML-2.4.2/include/ -std=c++11

Minion.o: Minion.cc
	g++ -c Minion.cc -I SFML-2.4.2/include/ -std=c++11

Tower.o: Tower.cc
	g++ -c Tower.cc -I SFML-2.4.2/include/ -std=c++11

LevelParser.o: LevelParser.cc
	g++ -c LevelParser.cc -I SFML-2.4.2/include/ -std=c++11

toweroffense: main.o TextureManager.o LevelParser.o Minion.o Tower.o
	g++ main.o TextureManager.o LevelParser.o Minion.o Tower.o -o toweroffense -L SFML-2.4.2/lib $(LIBS)

clean:
	rm -rf toweroffense.exe *.o

run:
	./toweroffense