/* main.c --- 
 * 
 * Filename: main.cpp
 * Description: Lab 5 for concurrent programming subject, the philosophers problem.
 * Author: Joseph
 * Maintainer: Joshua Wilkinson
 * Created: Wed Oct 11 09:28:12 2023 (+0100)
 * Last-Updated: Thu 19 Oct 11:20:22 BST 2023
 *           By: Joshua Wilkinson
 *     Update #: 14
 * 
 */

/* Commentary: 
 * 
 * file for lab 5, the goal of which is to solve the philospher's problem.
 * 
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Code: */

#include "Semaphore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h>

// set COUNT to however many philosophers you want to come to dinner 
const int COUNT = 5;//!< Number of philosophers
const int THINKTIME=3;//!< Thinking time modifier before forks are picked up by the Philosophers
const int EATTIME=5;//!< Eating time modifier before forks are put down by the Philosophers
std::vector<std::shared_ptr<Semaphore>> forks{COUNT};//!< Vector array of semaphores
std::shared_ptr<Semaphore> stopper;//!< For stopping greedy Philosophers!

/*! A philosopher thread is given a random time to think before searching for forks */
void think(int myID)
{
  int seconds=rand() % THINKTIME + 1;
  std::cout << myID << " is thinking! "<<std::endl;
  sleep(seconds);
}

/*! A philosopher searches for a fork to their left and to their right */
void get_forks(int philID)
{
  stopper->Wait(); // will block when 4 threads have reached this line
  std::cout << philID << " is getting their forks!" << std::endl; 
  forks[philID]->Wait();
  std::cout << philID << " has gotten their fork!" << std::endl; 
  forks[(philID+1)%COUNT]->Wait();
  std::cout << philID << " has gotten their fork!" << std::endl; 
}

/*! A philosopher puts down both of their forks, ready for the next philospher process */
void put_forks(int philID)
{
  std::cout << philID << " is putting their forks down!" << std::endl; 
  forks[philID]->Signal();
  forks[(philID+1)%COUNT]->Signal();
  stopper->Signal(); // will signal when a thread has finished eating, letting other processes go through
}

/*! The philosopher eats for a random amount of time */
void eat(int myID)
{
  int seconds=rand() % EATTIME + 1;
  std::cout << myID << " is chomping! "<<std::endl;
  sleep(seconds);  
}

/*! runs the dinner service for the philosophers for a set amount of time */
void philosopher(int id/* other params here*/)
{
  int times=10; // Number of times the philosphers sit down to eat!
  while(times>0){
    think(id);
    get_forks(id);
    eat(id);
    put_forks(id);
    times--;
  }//while  
}//philosopher

int main(void)
{
  /*! The first for loop iterates through the forks vector array to make the Semaphore property (1). */
  for (int i = 0; i < COUNT; ++i){
    forks[i] = std::make_shared<Semaphore>(1);
  }  
  srand (time(NULL)); // initialize random seed: 
  std::vector<std::thread> vt(COUNT);
  /*!  The stopper can withstand 4 waits before blocking */
  stopper = std::make_shared<Semaphore>(COUNT-1);
  int id=0;
  for(std::thread& t: vt){
    t=std::thread(philosopher,id++/*,params*/);
  }
  /** Join the philosopher threads with the main thread */
  for (auto& v :vt){
      v.join();
  }
  return 0;
}

/* main.cpp ends here */
