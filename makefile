LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all: toweroffense

main.o: main.cc
	g++ -c main.cc -I SFML-2.4.2/include/ -std=c++11

TextureManager.o: TextureManager.cc
	g++ -c TextureManager.cc -I SFML-2.4.2/include/ -std=c++11

toweroffense: main.o TextureManager.o
	g++ main.o TextureManager.o -o toweroffense -L SFML-2.4.2/lib $(LIBS)

clean:
	rm -rf toweroffense.exe *.o