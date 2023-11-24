# Name: Joshua Wilkinson
# Purpose: updated make file for lab 1
# Date: 08/10/2023
# License: GPL-3.0 license
#See http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for info on make files.

CC=g++
CFLAGS= -Wall
CPPFILES=Semaphore.cpp helloThreads.cpp

# each .o file depends on a .ccp file of the same name
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

ALL: $(CPPFILES)
	$(CC) -o mutualExclusion $(CPPFILES) $(CFLAGS) $(DEBUGFLAGS)

DEBUG: DEBUGFLAGS = -DDEBUG
DEBUG: ALL

doc:
	doxygen Doxyfile

CLEAN:
	rm *.o mutualExclusion
