# Name: Joshua Wilkinson
# Updated Make file
# Date: 12/10/2023                                                              
# Purpose: Make file for lab 4, producer consumer problem.     
# See http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for info on make files.                          

CC=g++
CPPFILES=SafeBuffer.cpp Event.cpp Semaphore.cpp main.cpp
HFILES=Barrier.h Semaphore.h SafeBuffer.h Event.h Buffer.h
CFLAGS= -std=c++11 -Wall -pthread -I.
OBJFILES=Barrier.o main.o Semaphore.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

ALL: $(OBJFILES)
	$(CC) -o ProducerConsumer SafeBuffer.cpp Event.cpp Semaphore.cpp main.cpp $(CFLAGS) $(DEBUGFLAGS)

DEBUG: DEBUGFLAGS = -g -O0
DEBUG: ALL

doc:
	doxygen Doxyfile

CLEAN:
	rm *.o
