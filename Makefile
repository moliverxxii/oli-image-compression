SOURCE = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCE:.c=.o)
BIN = bin

all: compressor

compressor: $(OBJECTS)
	gcc -o $@ $^ -lm

obj/%.o : src/%.c src/main.h
	gcc -c -o $@ $< 

src/main.h: $(HEADERS)
	touch src/main.h

clean:
	rm obj/*.o compressor -f
	
