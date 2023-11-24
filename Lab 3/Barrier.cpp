#include "Barrier.h"

/*! Barrier constructor with a paramater for the number of threads*/
Barrier::Barrier(int NumOfThreads)
{
  this->count = NumOfThreads;
  threadNum = 0;
  condition = false;
  theMutex=std::make_shared<Semaphore>(1);
  innerLock=std::make_shared<Semaphore>(0);
  outerLock=std::make_shared<Semaphore>(1);
}
/*! Barrier deconstructor*/
Barrier::~Barrier()
{
    // nothing to do
}

/*! sets count value*/
void Barrier::setCount(int x)
{
  this->count = x;
}
/*! returns count value*/
int Barrier::getCount()
{
  return this->count;
}

/*! waits for all the threads before starting second half of code*/ 
void Barrier::waitForAll()
{
  theMutex->Wait();
  threadNum++;

  if(threadNum == count){
    outerLock->Wait();
    innerLock->Signal();
  }
  theMutex->Signal();
  innerLock->Wait();
  innerLock->Signal();
  theMutex->Wait();
  threadNum--;
  if (threadNum == 0){
    innerLock->Wait();
    outerLock->Signal();
  }
  theMutex->Signal();
  outerLock->Wait();
  outerLock->Signal();
}
