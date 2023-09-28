// Name: Joshua Wilkinson
// Student Number: C00262503
// Date: 25/09/2023
// Purpose: to apply a rendezvous barrier as a buffer so that "first" is printed before "second"

#include "Semaphore.h"
#include <thread>
#include <vector>
#include <iostream>

/*! displays the first function in the barrier being executed */
void task(std::shared_ptr<Semaphore> mutexSem,std::shared_ptr<Semaphore> barrierSem, std::shared_ptr<int> threadCount){
  //template< typename R, typename P >;
  //std::chrono_literals s;
  //int mutexSem.get()->Wait(5s);  

  mutexSem.get()->Wait();
  std::cout << "first " << std::endl;
  
  //barrier here
  /*
  if (*threadCount == 0){
    barrierSem.get()->Signal();
  }
  else{
    barrierSem.get()->Wait();
  }  
  */
  mutexSem.get()->Signal();
  std::cout << "second" << std::endl;

  // end of barrier
  
  *threadCount = *threadCount - 1;
}




int main(void){
  std::shared_ptr<Semaphore> mutexSem;
  std::shared_ptr<Semaphore> barrierSem;
  std::shared_ptr<int> threadCount;
  mutexSem=std::make_shared<Semaphore>(1);
  barrierSem=std::make_shared<Semaphore>(0);
  threadCount=std::make_shared<int>(5);
  /*!< An array of threads*/
  std::vector<std::thread> threadArray(*threadCount);
  /*!< Pointer to barrier object*/

  for(long unsigned int i=0; i < threadArray.size(); i++){
    threadArray[i]=std::thread(task,mutexSem,barrierSem,threadCount);
  }

  for(long unsigned int i = 0; i < threadArray.size(); i++){
    threadArray[i].join();
  }
  
  return 0;
}
