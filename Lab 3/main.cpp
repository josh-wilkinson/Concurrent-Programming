// Name: Joshua Wilkinson
// Student Number: C00262503
// 08/10/2023
// Purpose: driver to run Lab 3 and check if the barrier is working properly

#include "Barrier.h"
#include <thread>
#include <vector>
const int TotalThreads = 5;
/*! displays the first function in the barrier being executed */
void task(std::shared_ptr<Barrier> barrierObj)
{
  for (int i = 0; i < 5; ++i){
    std::cout << "first " << std::endl;
    barrierObj->waitForAll();
    std::cout << "second" << std::endl;
    barrierObj->waitForAll();
  }
}
int main(void)
{
  std::vector<std::thread> threadArray(TotalThreads); /*! An array of threads*/
  std::shared_ptr<Barrier> barrierObj( new Barrier(5)); /*! Pointer to Barrier object*/

  barrierObj->setCount(5);
  /*! Setting up array of threads*/
  for(int i=0; i < threadArray.size(); ++i){
    threadArray[i]=std::thread(task,barrierObj);
  }
  /*! Joining threads*/
  for(int i = 0; i < threadArray.size(); i++){
    threadArray[i].join();
  }
  
  return 0;
}
