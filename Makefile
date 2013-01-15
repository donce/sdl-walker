CPPFLAGS = -lSDL -I /usr/include/SDL/ -g

walker: walker.o segment.o point.o
	g++ -o walker walker.o point.o segment.o $(CPPFLAGS) 

run: walker
	./walker
clean:
	rm -f *.o walker
