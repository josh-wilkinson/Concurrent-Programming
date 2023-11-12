#include "Barrier.h"
#include "SafeBuffer.h"
#include "Event.h"
#include <iostream>
#include <thread>
#include <vector>


static const int num_threads = 100;
const int size=20;

/*! \fn producer
    \brief Creates events and adds them to buffer
*/

void producer(std::shared_ptr<SafeBuffer> theBuffer, int numLoops){

  for(int i = 0; i < numLoops; ++i){    
    //Produce event and add to buffer
    Event e;
    theBuffer->push(e);
  } 

}

/*! \fn consumer
    \brief Takes events from buffer and consumes them
*/

void consumer(std::shared_ptr<SafeBuffer> theBuffer, int numLoops){

  for(int i = 0; i < numLoops; ++i){
    //Produce event and add to buffer
    Event e;
    e = theBuffer->pop();
    e.consume();
  }  

}

int main(void){
  std::vector<std::thread> producers(num_threads);
  std::vector<std::thread> consumers(num_threads);
  std::shared_ptr<SafeBuffer> aBuffer(new SafeBuffer());
  
  /**< Launch the threads  */
  
  for(std::thread& p: producers){
    p = std::thread(producer,aBuffer,10);
  }
  for(std::thread& c: consumers){
    c = std::thread(consumer,aBuffer,10);
  }
	  
  /**< Join the threads with the main thread */
  
  for (auto& p :producers){
      p.join();
  }
  for (auto& c :consumers){
      c.join();
  }
  
  //std::cout << std::endl;
  return 0;
}
