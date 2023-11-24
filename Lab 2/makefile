# Name: Joshua Wilkinson
# Updated Make file
# Date: 12/10/2023                                                              
# Purpose: Make file for lab 2, rendezvous barrier.
# License: GPL-3.0 license
# See http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for info on make files.

CC=g++
CFLAGS= -Wall -o rendezvous
CPPFILES=Semaphore.cpp rendezvous.cpp 

RENDEZVOUS: $(CPPFILES)
	$(CC) $(CPPFILES) $(CFLAGS)

doc:
	doxygen Doxyfile

CLEAN:
	rm rendezvous *.o *.out
