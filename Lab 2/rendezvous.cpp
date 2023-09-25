// Name: Joshua Wilkinson
// Student Number: C00262503
// Date: 25/09/2023
// Purpose: to apply a rendezvous barrier as a buffer so that "first" is printed before "second"

#include "Semaphore.h"
#include <thread>
#include <vector>
#include <iostream>

/*! displays the first function in the barrier being executed */
void task(std::shared_ptr<Semaphore> mutexSem,std::shared_ptr<Semaphore> barrierSem, int threadCount){

  std::cout << "first " << std::endl;

  
  //put barrier code here
  std::cout << "second" << std::endl;
}




int main(void){
  std::shared_ptr<Semaphore> mutexSem;
  std::shared_ptr<Semaphore> barrierSem;
  int threadCount = 5;
  mutexSem=std::make_shared<Semaphore>(1);
  barrierSem=std::make_shared<Semaphore>(0);
  /*!< An array of threads*/
  std::vector<std::thread> threadArray(threadCount);
  /*!< Pointer to barrier object*/

  for(long unsigned int i=0; i < threadArray.size(); i++){
    threadArray[i]=std::thread(task,mutexSem,barrierSem,threadCount);
  }

  for(long unsigned int i = 0; i < threadArray.size(); i++){
    threadArray[i].join();
  }
  
  return 0;
}
