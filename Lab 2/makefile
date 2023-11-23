# Name: Joshua Wilkinson
# Updated Make file
# Date: 12/10/2023                                                              
# Purpose: Make file for lab 2, rendezvous barrier.

CC=g++
CFLAGS= -Wall -o fred
CPPFILES=Semaphore.cpp rendezvous.cpp 

RENDEZVOUS: $(CPPFILES)
	$(CC) -o rendezvous $(CPPFILES) $(CFLAGS)

doc:
	doxygen Doxyfile

CLEAN:
	rm fred *.o *.out
