# Name: Joshua Wilkinson
# Date: 18/10/2023
# Purpose: Make file for lab 5
# License: GPL-3.0 license
# See http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for info on make files.

CC=g++
CFLAGS= -std=c++11 -Wall -pthread -I.
CPPFILES=main.cpp Semaphore.cpp

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

SIMPLE: $(CPPFILES)
	$(CC) -o DiningPhilosophers $(CFLAGS) $(CPPFILES)

doc:
	doxygen Doxyfile

CLEAN:
	rm *.o DiningPhilosophers
