all:
	windres -i ikonka.rc -o ikonka.o
	g++ -Wall -g -c nurbs.cpp
	g++ -Wall -g -c projection.cpp
	g++ -Wall -g -c ioparametru.cpp
	g++ -I src/include -L src/lib -o NURBS-visualizer main.cpp nurbs.o projection.o ioparametru.o ikonka.o -lmingw32 -lSDL2main -lSDL2