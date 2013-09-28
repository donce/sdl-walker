CPPFLAGS = -lSDL -I /usr/include/SDL/ -g

OBJ_FILES = walker.o game.o world.o screen.o image.o object.o enemy.o segment.o point.o
BINARY = walker

$(BINARY): $(OBJ_FILES)
	g++ -o $(BINARY) $(OBJ_FILES) $(CPPFLAGS) 
run: $(BINARY)
	./$(BINARY)
clean:
	rm -f *.o $(BINARY)
