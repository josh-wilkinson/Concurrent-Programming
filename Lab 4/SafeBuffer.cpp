#include "SafeBuffer.h"
#include <stdlib.h>
#include "Event.h"

/*! \class SafeBuffer
    \brief A thread safe implementation of a buffer
*/

/*! \fn buffer constructor
    \brief Initialises the semaphores
*/
SafeBuffer::SafeBuffer()
{
  /**
   * Mutex variable
   * theMutex provides exclusive access to the buffer. 
   */
  theMutex = std::make_shared<Semaphore>(1);
  /**
   * When semaphore is positive, it indicates the number of items in the buffer.
   * When it is negative, it indicates the number of consumer threads in the queue.
   */
  semaphore = std::make_shared<Semaphore>(0);
}

/*! \fn buffer producer
    \brief Takes an event and adds them to buffer
*/
void SafeBuffer::push(Event theEvent)
{
  /**
   * In order to avoid deadlock, producers and consumers check availability before
   * getting the mutex. For the best performance, they release the mutex before signaling.
   */
  theMutex->Wait();
  // use vector methods
  items.push_back(theEvent);
  theMutex->Signal();
  semaphore->Signal();
}

/*! \fn buffer consumer
    \brief Removes an event from the buffer and returns an instance of it
*/
Event SafeBuffer::pop()
{
  /**
   * If a consumer thread arrives when the buffer is empty, it blocks until
   * a producer adds a new item.
   */
  semaphore->Wait();
  theMutex->Wait();
  Event theEvent = items.back();
  items.pop_back();
  theMutex->Signal();
  return theEvent;
}
