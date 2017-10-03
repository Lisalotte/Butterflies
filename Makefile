CC=g++
CFLAGS=-Wall `sdl-config --cflags`
LDFLAGS=-lGL -lglut -lGLU -lSDL -lSDL_mixer -lpng -lz
SOURCES=main.cpp UserInput.cpp light.cpp interface.cpp geometry.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=./Butterflies

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)
