SHELL:=/bin/bash 

CC=gcc
CFLAGS=-x c -c -std=c99 -Wall -Wextra -pedantic -Werror -g -ggdb3
LDFLAGS=-lm
SOURCES=main.c definitions.c tools.c readfile.c parsejpeg.c  decodejpeg.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=definitions.h tools.h readfile.h parsejpeg.h decodejpeg.h 
DEPS=$(HEADERS:.h=.d)
EXECUTABLE=main

VFLAGS=-v --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXECUTABLE) > log.txt 2>&1

.PHONY: all clean monitor

all: $(SOURCES) $(EXECUTABLE)


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


clean:
	rm -f *.o *.d *~ $(EXECUTABLE)

monitor:
	-@make
	-./$(EXECUTABLE)
	@echo "Files watched: " $(SOURCES) $(HEADERS)
	@echo "Executable: ./" $(EXECUTABLE)
	@while [[ 1 ]]; do inotifywait -e modify $(SOURCES) $(HEADERS); make && ./$(EXECUTABLE); valgrind $(VFLAGS); done
