remake: clean all

all: geomelt 

geomelt:
	g++ -std=c++17 -o geoMelt assets.cpp camera.cpp command.cpp complex_shapes.cpp input.cpp levels.cpp menus.cpp player.cpp primitives.cpp scenery.cpp state.cpp sync.cpp -lsfml-graphics -lsfml-window -lsfml-system -Wall -lX11 -lGL -lGLU -lm

clean: 
	rm -f geomelt
	rm -f *.o
