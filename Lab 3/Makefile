# Name: Joshua Wilkinson
# Date: 02/10/2023
# Purpose:

CC=g++
CPPFILES=Barrier.cpp main.cpp Semaphore.cpp
HFILES=Barrier.h Semaphore.h
CFLAGS= -std=c++11 -pthread -I.
OBJFILES=Barrier.o main.o Semaphore.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

ALL: $(OBJFILES) $(HFILES)
	$(CC) -o barrier $(OBJFILES) $(CFLAGS) $(DEBUGFLAGS) 

DEBUG: DEBUGFLAGS = -g -O0
DEBUG: ALL

CLEAN:
	rm *.o
