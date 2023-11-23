# Name: Joshua Wilkinson
# Purpose: simple make file for lab 1
# Date: 08/10/2023
# License: GPL-3.0 license

CC=g++
CFLAGS= -Wall -o josh
CPPFILES=Semaphore.cpp helloThreads.cpp


HELLOTHREADS: $(CPPFILES)
	$(CC) $(CPPFILES) $(CFLAGS)

doc:
	doxygen Doxyfile

CLEAN:
	rm *.o
