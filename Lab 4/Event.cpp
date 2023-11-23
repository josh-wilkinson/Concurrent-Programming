#include "Event.h"
#include <iostream>
#include <stdlib.h>

/*! \class Event
    \brief An implementation of Event

   Stores a random char.

*/

Event::Event()
{
  c = 'a' + rand() % 26; // there are 26 possible letters
}

Event::~Event()
{
  //nothing to do
}

// the variable must be outputed in upper case
void Event::consume()
{
  char cToUpperChar = c-32;
  std::cout << cToUpperChar << std::endl;
}
