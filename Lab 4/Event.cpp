#include "Event.h"
#include <iostream>
#include <stdlib.h>

/*! \class Event
    \brief An implementation of Event

   Stores a random char.

*/

Event::Event()
{
  /*! Constructor method */
  /*! Uses the rand() function to generate a random number out of 26 (for each letter in the alphabet) */
  c = 'a' + rand() % 26; // there are 26 possible letters
}

Event::~Event()
{
  /*! Destructor method */
  //nothing to do
}

// the variable must be outputed in upper case
void Event::consume()
{
  /*! Prints out to console the Event char in uppercase. */
  char cToUpperChar = c-32;
  std::cout << cToUpperChar << std::endl;
}
