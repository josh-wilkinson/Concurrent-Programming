#include "SafeBuffer.h"
#include <iostream>
#include <stdlib.h>
#include "Event.h"

//-------------------------------------------------------------------
// Buffer (Class)
//     The buffer is implemented with a Event class.
//-------------------------------------------------------------------

SafeBuffer::SafeBuffer(){
  theMutex = std::make_shared<Semaphore>(1);
  semaphore = std::make_shared<Semaphore>(0);
}

void SafeBuffer::push(Event theEvent){
  // use vector methods
  items.push_back(theEvent);
}

Event SafeBuffer::pop(){
  Event theEvent = items.back();
  items.pop_back();
  return theEvent;
}
