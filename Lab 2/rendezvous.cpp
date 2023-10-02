// Name: Joshua Wilkinson
// Student Number: C00262503
// Date: 25/09/2023
// Purpose: to apply a rendezvous barrier as a buffer so that "first" is printed before "second"

#include "Semaphore.h"
#include <thread>
#include <vector>
#include <iostream>

/*! displays the first function in the barrier being executed */
void task(std::shared_ptr<Semaphore> mutexSem,std::shared_ptr<Semaphore> barrierSem, std::shared_ptr<int> threadCount, std::shared_ptr<int> firstArrived){  
  mutexSem->Wait();
  *firstArrived += 1;
  //std::cout << "Arrival number: " << *firstArrived << std::endl;
  mutexSem->Signal();  
  
  std::cout << "first " << std::endl;
  
  //barrier here
  
  if (*firstArrived < *threadCount){
    mutexSem->Signal();
    barrierSem->Wait();
  }
  else{
    barrierSem->Signal();
    mutexSem->Wait();
  }

  mutexSem->Wait();
  *firstArrived -= 1;
  //std::cout << "Arrival number: " << *firstArrived << std::endl;
  mutexSem->Signal();

  if (*firstArrived >= 1){
    barrierSem->Signal();
  }
  
  std::cout << "second" << std::endl;

  // end of barrier
  
  
}




int main(void){
  std::shared_ptr<Semaphore> mutexSem;
  std::shared_ptr<Semaphore> barrierSem;

  std::shared_ptr<int> firstArrived;
  std::shared_ptr<int> secondArrived;
  
  std::shared_ptr<int> threadCount;
  
  mutexSem=std::make_shared<Semaphore>(1);
  barrierSem=std::make_shared<Semaphore>(0);
  firstArrived=std::make_shared<int>(0);
  secondArrived=std::make_shared<int>(0);
  threadCount=std::make_shared<int>(5);
  /*!< An array of threads*/
  std::vector<std::thread> threadArray(*threadCount);
  /*!< Pointer to barrier object*/

  for(long unsigned int i=0; i < threadArray.size(); i++){
    threadArray[i]=std::thread(task,mutexSem,barrierSem,threadCount, firstArrived);
  }

  for(long unsigned int i = 0; i < threadArray.size(); i++){
    threadArray[i].join();
  }
  
  return 0;
}
