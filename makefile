# Makefile for making of the roller shutter controller programm
#
#

# Compiler to use
CC=g++

# Options for the compiler
CFLAGS=-c -Wall -std=c++0x

# Additional libraries
LDFLAGS=-lwiringPi -pthread

# Here are the Source-Files that are needed to build the application
SOURCES=main.cpp rollershuttermotor.cpp Parsino.cpp InputCTRL.cpp

OBJECTS=$(SOURCES:.cpp=.o)

# Here is the target of the make file
EXECUTABLE=roller_shutter_ctrl

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

# Clean
.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

# Installation of the program
BINDIR = $(DESTDIR)/usr/bin

.PHONY: install
install: $(EXECUTABLE)
	mkdir -p $(PREFIX)/bin
	cp $< $(PREFIX)/bin/$(EXECUTABLE)

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin/$(EXECUTABLE)



