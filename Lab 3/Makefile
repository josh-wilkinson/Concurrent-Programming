# Name: Joshua Wilkinson
# Date: 02/10/2023
# Purpose: Make file for lab 3, reusable barrier.
# License: GPL-3.0 license
# See http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for info on make files.

CC=g++
CPPFILES=Barrier.cpp main.cpp Semaphore.cpp
HFILES=Barrier.h Semaphore.h
CFLAGS= -std=c++11 -Wall -pthread -I.
OBJFILES=Barrier.o main.o Semaphore.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

ALL: $(OBJFILES) $(HFILES)
	$(CC) -o barrier $(OBJFILES) $(CFLAGS) $(DEBUGFLAGS) 

DEBUG: DEBUGFLAGS = -g -O0
DEBUG: ALL

doc:
	doxygen Doxyfile

CLEAN:
	rm *.o barrier
