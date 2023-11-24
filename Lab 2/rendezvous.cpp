// Name: Joshua Wilkinson
// Student Number: C00262503
// Date: 25/09/2023
// Purpose: to apply a rendezvous barrier as a buffer so that "first" is printed before "second"

#include "Semaphore.h"
#include <thread>
#include <vector>
#include <iostream>

/*! \fn task
  \brief The function being executed by the threads.
 */
void task(std::shared_ptr<Semaphore> mutexSem,std::shared_ptr<Semaphore> barrierSem, std::shared_ptr<int> threadCount, std::shared_ptr<int> firstArrived)
{
  /*! 
    A mutex Semaphore is applied when the function is being executed. 
    Then it is release when all the threads have completed the first half.
   */  
  mutexSem->Wait();
  *firstArrived += 1;
  //std::cout << "Arrival number: " << *firstArrived << std::endl;  
  
  std::cout << "first " << std::endl;
  
  //barrier here  
  if (*firstArrived == *threadCount){
    barrierSem->Signal();
  }
  mutexSem->Signal();

  barrierSem->Wait();
  barrierSem->Signal();

  // end of barrier

  std::cout << "second" << std::endl;  
}




int main(void)
{
  /*! Four shared pointers: Two semaphores and Two integers. */
  std::shared_ptr<Semaphore> mutexSem;
  std::shared_ptr<Semaphore> barrierSem;

  std::shared_ptr<int> firstArrived;  
  std::shared_ptr<int> threadCount;
  
  mutexSem=std::make_shared<Semaphore>(1);
  barrierSem=std::make_shared<Semaphore>(0);
  firstArrived=std::make_shared<int>(0);
  threadCount=std::make_shared<int>(5);
  /*! And an array of threads the size of the thread count. */
  std::vector<std::thread> threadArray(*threadCount);

  /*! Then the threads run the task method. */

  for(long unsigned int i=0; i < threadArray.size(); i++){
    threadArray[i]=std::thread(task, mutexSem, barrierSem, threadCount, firstArrived);
  }

  for(long unsigned int i = 0; i < threadArray.size(); i++){
    threadArray[i].join();
  }
  
  return 0;
}
