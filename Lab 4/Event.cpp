#include "Event.h"
#include <iostream>
#include <stdlib.h>

Event::Event(Buffer<std::shared_ptr Event> theBuffer){
  c = 'a' + rand() % 26; // there are 26 possible letters
}

Event::~Event(){
  //nothing to do
}

// the variable must be outputed in upper case
void Event::consume(){
  char cToUpperChar = c-32;
  std::cout << cToUpperChar << std::endl;
}
