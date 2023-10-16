/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Joseph
 * Maintainer: 
 * Created: Wed Oct 11 09:28:12 2023 (+0100)
 * Last-Updated: Wed Oct 11 10:01:39 2023 (+0100)
 *           By: Joseph
 *     Update #: 13
 * 
 */

/* Commentary: 
 * 
 * 
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

const int COUNT = 5;
const int THINKTIME=3;
const int EATTIME=5;
std::vector<std::shared_ptr<Semaphore>> forks(COUNT);
std::shared_ptr<Semaphore> stopper;
//std::shared_ptr<Semaphore> dummy;

void think(int myID){
  //stopper->Wait();
  int seconds=rand() % THINKTIME + 1;
  std::cout << myID << " is thinking! "<<std::endl;
  sleep(seconds);
}

void get_forks(int philID){
  stopper->Wait();
  std::cout << philID << " is getting their forks!" << std::endl; 
  forks[philID]->Wait();
  std::cout << philID << " has gotten their fork!" << std::endl; 
  forks[(philID+1)%COUNT]->Wait();
  std::cout << philID << " has gotten their fork!" << std::endl; 
}

void put_forks(int philID){
  std::cout << philID << " is putting their forks down!" << std::endl; 
  forks[philID]->Signal();
  forks[(philID+1)%COUNT]->Signal();
  stopper->Signal();
}

void eat(int myID){
  int seconds=rand() % EATTIME + 1;
  std::cout << myID << " is chomping! "<<std::endl;
  sleep(seconds);  
}

void philosopher(int id/* other params here*/){
  int times=1;
  while(times>0){
    think(id);
    get_forks(id);
    eat(id);
    put_forks(id);
    times--;
  }//while  
}//philosopher



int main(void){

  for (int i = 0; i < COUNT; ++i){
    forks[i] = std::make_shared<Semaphore>(1);
  }
  
  srand (time(NULL)); // initialize random seed: 
  std::vector<std::thread> vt(COUNT);
  stopper = std::make_shared<Semaphore>(4); // can withstand 4 waits before blocking
  int id=0;
  for(std::thread& t: vt){
    t=std::thread(philosopher,id++/*,params*/);
  }
  /**< Join the philosopher threads with the main thread */
  for (auto& v :vt){
      v.join();
  }
  return 0;
}

/* main.c ends here */
