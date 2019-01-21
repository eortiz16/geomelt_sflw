remake: clean all

all: geomelt 

geomelt: geomelt.cpp
	g++ -std=c++17 -o geoMelt assets.cpp camera.cpp complex_shapes.cpp geomelt.cpp levels.cpp menus.cpp player.cpp primitives.cpp -lsfml-graphics -lsfml-window -lsfml-system -Wall -lX11 -lGL -lGLU -lm

clean: 
	rm -f geomelt
	rm -f *.o
