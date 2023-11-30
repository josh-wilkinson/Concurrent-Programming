// test.cpp --- 
// 
// Filename: test.cpp
// Description: 
// Author: Joseph
// Maintainer: 
// Created: Fri Nov  3 15:51:15 2023 (+0000)
// Last-Updated: Fri Nov  3 16:49:06 2023 (+0000)
//           By: Joseph
//     Update #: 19
// 
// 

// Commentary: 
// 
// 
// 
// 
// 
// 
// 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
// 
// some helpful code here!
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// https://www.sfml-dev.org/tutorials/2.5/start-linux.php
// https://learnsfml.com/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Code:

#include <SFML/Graphics.hpp>
#include <stdlib.h> /* srand, rand */
#include <time.h>
#include <chrono>
#include <unistd.h> /* sleep() */
#include <iostream> /* for debugging */
#include <omp.h> /* for openMp */

/* Struct */
struct Square
{
  int value; // 0 = ocean, 1 = fish, 2 = shark.
  int sharkStarveProgress = 0;
  int age = 0;
};

/* Variables */
const int NUMCORES=2;//My core count -- change as required
int NumThreads = 2; // number of threads used by OpenMP 
// window parameters
const int xdim = 100;
const int ydim = 100;
// dynamic parameters
const unsigned int fishBreed = 3;
const unsigned int sharkBreed = 6;
const int starve = 4;
//each shape will represent either a fish, shark or empty space
//e.g. blue for empty, red for shark and green for fish
sf::RectangleShape recArray[xdim][ydim];  
Square worldData[xdim][ydim]; //contains a 0(nothing), 1(fish) or a 2(shark).
Square restartData[xdim][ydim];
int WindowXSize=800;
int WindowYSize=600;
int cellXSize=WindowXSize/xdim;
int cellYSize=WindowYSize/ydim;
int movePosition;
int numShark = 0;
int numFish = 0;
int fishCounter = 0;
int sharkCounter = 0;
bool paused = true;
bool makeMoreFish = false;
bool makeMoreSharks = false;
// ratio for initial fish/shark population
const float fishRatio = 0.05;
const float sharkRatio = 0.005;
//delay
const float updateDelay = 0.1;

/* Thread methods */

///! Find out how many threads are running!
int get_num_threads(void) {
  int num_threads = NumThreads;
  //must ask in parallel region otherwise 1 is returned
#pragma omp parallel
  {
    //#pragma omp single
    num_threads = omp_get_num_threads();
  }
  return num_threads;
}

/* Methods */

void populate()
{
  numFish = 0;
  numShark = 0;
  std::cout << "Placing sharks and fishes..." << std::endl;
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){      
      float randomNumber = (rand() % 10000) / 10000.0; // random number between 0 and 1 (fish or shark)      
      //std::cout << randomNumber;      
      if (randomNumber <= fishRatio){
	worldData[i][k].value = 1;
	numFish++;
      }
      else if (randomNumber > fishRatio && randomNumber < fishRatio + sharkRatio){
        worldData[i][k].value = 2;
	numShark++;	
      }
      else{
	worldData[i][k].value = 0;
      }
      worldData[i][k].sharkStarveProgress = 0;
    }
    //std::cout << std::endl;
  }
}

void copyArray(Square array[xdim][ydim], Square copyArray[xdim][ydim]){
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      copyArray[i][k].value = array[i][k].value;
      copyArray[i][k].age = array[i][k].age;
      copyArray[i][k].sharkStarveProgress = array[i][k].sharkStarveProgress;
    }
  }
}

void restart(){
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      worldData[i][k] = restartData[i][k];
    }
  }
}

void update()
{
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      // update colours
      if (worldData[i][k].value == 1)
	recArray[i][k].setFillColor(sf::Color::Green);
      else if (worldData[i][k].value == 2)
	recArray[i][k].setFillColor(sf::Color::Red);
      else
	recArray[i][k].setFillColor(sf::Color::Blue);
    }
  }
}

void moveFish(int x, int y)
{
  // check for fish
  if (worldData[x][y].value != 1)
    return;
  
  int xFree[8] = {0};
  int yFree[8] = {0};

  int xIndex = 0;
  int yIndex = 0;

  int oldPositionAge = 0;
  int newPositionAge = 0;

  int freeBlocksCounter = 0; // to count all the free adjacent blocks

  bool breeding = false;
  
  // fish age / breeding condition
  if (worldData[x][y].age >= fishBreed){
    oldPositionAge = 1;
    newPositionAge = 1;
    breeding = true;
  }
  else{
    oldPositionAge = 0;
    newPositionAge = worldData[x][y].age + 1;
  }
    
  // nested for loop to go through all neighbouring blocks
  for (int i = -1; i < 2; i++){
    for (int k = -1; k < 2; k++){
      // avoid itself
      if (i*i + k*k != 0){

	xIndex = x + i;
	yIndex = y + k;

	// boundary check
	if (xIndex < 0)
	  xIndex = xdim-1;
	if (xIndex >= xdim)
	  xIndex = 0;
	if (yIndex < 0)
	  yIndex = ydim-1;
	if (yIndex >= ydim)
	  yIndex = 0;

	// record position of empty block
	if (worldData[xIndex][yIndex].value == 0){
	  // fill the 2 different arrays with an empty block location (sea)
	  xFree[freeBlocksCounter] = xIndex;
	  yFree[freeBlocksCounter] = yIndex;
	  freeBlocksCounter += 1;
	}
	
      }
    }
  }

  // now check here if we found any free neighbouring blocks
  if (freeBlocksCounter != 0){
    int randomFreeBlockNumber = rand() % freeBlocksCounter; // get the index for xFree & yFree

    unsigned int xpos = xFree[randomFreeBlockNumber];
    unsigned int ypos = yFree[randomFreeBlockNumber];

    // now update worldData
    worldData[xpos][ypos].value = 1; // move fish
    if (breeding)
      worldData[x][y].value = 1;
    else
      worldData[x][y].value = 0;
    
    // age stuff
    worldData[xpos][ypos].age = newPositionAge;
    worldData[x][y].age = oldPositionAge;
  }
  else{
    // the fish has not found an empty block, so only the age should be updated
    worldData[x][y].age = newPositionAge;
  }
  
}

void moveShark(int x, int y)
{
  // check for shark
  if (worldData[x][y].value != 2)
    return;

  // Arrays to store empty blocks
  int xFree[8] = {0};
  int yFree[8] = {0};

  // Arrays to store fish blocks
  int xFish[8] = {0};
  int yFish[8] = {0};

  int xIndex = 0;
  int yIndex = 0;

  int oldPositionAge = 0;
  int newPositionAge = 0;

  int freeBlocksCounter = 0; // to count all the free adjacent blocks
  int fishBlocksCounter = 0; // to count all the adjacent fish occupied blocks (mmm... tasty)

  bool breeding = false;
  
  // shark age / breeding condition
  if (worldData[x][y].age >= sharkBreed){
    oldPositionAge = 1;
    newPositionAge = 1;
    breeding = true;
  }
  else{
    oldPositionAge = 0;
    newPositionAge = worldData[x][y].age + 1;
  }
    
  // nested for loop to go through all neighbouring blocks
  for (int i = -1; i < 2; i++){
    for (int k = -1; k < 2; k++){
      // avoid itself
      if (i*i + k*k != 0){

	xIndex = x + i;
	yIndex = y + k;

	// boundary check
	if (xIndex < 0)
	  xIndex = xdim-1;
	if (xIndex >= xdim)
	  xIndex = 0;
	if (yIndex < 0)
	  yIndex = ydim-1;
	if (yIndex >= ydim)
	  yIndex = 0;

	// record fish blocks
	if (worldData[xIndex][yIndex].value == 1){
	  xFish[fishBlocksCounter] = xIndex;
	  yFish[fishBlocksCounter] = yIndex;
	  fishBlocksCounter += 1;
	  //std::cout << fishBlocksCounter;
	}
	
	// record position of empty block
	if (worldData[xIndex][yIndex].value == 0){
	  // fill the 2 different arrays with the x & y positions of the empty block location (sea)
	  xFree[freeBlocksCounter] = xIndex;
	  yFree[freeBlocksCounter] = yIndex;
	  freeBlocksCounter += 1;
	}
	
      }
    }
  }

  // check here to see if we found any adjacent fish blocks
  if (fishBlocksCounter != 0){
    //std::cout << "Found fish" << std::endl;
    int randomFishBlockNumber = rand() % fishBlocksCounter; // get the indexes
    //std::cout << randomFishBlockNumber;

    unsigned int xpos = xFish[randomFishBlockNumber];
    unsigned int ypos = yFish[randomFishBlockNumber];

    //std::cout << xpos << " " << ypos << std::endl;

    worldData[xpos][ypos].value = 0; // kill fish

    // move shark
    worldData[xpos][ypos].value = 2;
    if (breeding){
      // new shark
      worldData[x][y].value = 2;
      worldData[x][y].sharkStarveProgress = 1;
    }
    else{
      worldData[x][y].value = 0;
      worldData[x][y].sharkStarveProgress = 0;
    }

    // starvation stuff
    worldData[xpos][ypos].sharkStarveProgress = 1;
        
    // age stuff
    worldData[xpos][ypos].age = newPositionAge;
    worldData[x][y].age = oldPositionAge;
  }  
  // check here if we cannot find any fish but there's an empty block adjacent
  if (freeBlocksCounter != 0 && fishBlocksCounter == 0){
    int randomFreeBlockNumber = rand() % freeBlocksCounter; // get the index for xFree & yFree

    unsigned int xpos = xFree[randomFreeBlockNumber];
    unsigned int ypos = yFree[randomFreeBlockNumber];

    // move shark
    worldData[xpos][ypos].value = 2;

    // starvation stuff
    worldData[xpos][ypos].sharkStarveProgress = worldData[x][y].sharkStarveProgress + 1;
    
    if (breeding){
      // new shark
      worldData[x][y].value = 2;
      worldData[x][y].sharkStarveProgress = 1;
    }
    else{
      worldData[x][y].value = 0;
      worldData[x][y].sharkStarveProgress = 0;
    }
        
    // age stuff
    worldData[xpos][ypos].age = newPositionAge;
    worldData[x][y].age = oldPositionAge;
    
    if (worldData[xpos][ypos].sharkStarveProgress == starve){
      worldData[xpos][ypos].value = 0; // RIP goodnight sweet prince
      worldData[xpos][ypos].sharkStarveProgress = 0; // reset starve progress
      worldData[xpos][ypos].age = 0; // reset age
    }
  }  
  // check if nothing is found
  if (fishBlocksCounter == 0 && freeBlocksCounter == 0){
    // no movement
    worldData[x][y].age = newPositionAge;
    // increase shark starve progress
    worldData[x][y].sharkStarveProgress += 1;
    // now check to see if the shark dies
    if (worldData[x][y].sharkStarveProgress >= starve){
      worldData[x][y].value = 0; // RIP goodnight sweet prince
      worldData[x][y].sharkStarveProgress = 0; // reset starve progress
      worldData[x][y].age = 0; // reset age
    }
  }
  
}  


void move()
{
#pragma omp parallel num_threads(8)
  {//parallel start
    int tid = omp_get_thread_num();
    int tileRowSize = (xdim/omp_get_num_threads());

    
    
    int rowBlocksStart = tid * tileRowSize;    
    int columnBlocksStart = 0;    
    int rowBlocksEnd = rowBlocksStart + tileRowSize;
    int columnBlocksEnd = ydim;
    
    if (rowBlocksEnd > xdim)
      rowBlocksEnd = xdim;
    if (columnBlocksEnd > ydim)
      columnBlocksEnd = ydim;
    if ((omp_get_num_threads() == omp_get_thread_num()+1) && rowBlocksEnd < xdim){
      //std::cout << xdim;
      rowBlocksEnd = xdim + 1;
    }
    
    for (int i = columnBlocksStart+1; i < columnBlocksEnd-1; ++i){//columns i
      for (int k = rowBlocksStart+1; k < rowBlocksEnd-1; ++k){//rows k
	switch(worldData[i][k].value){
	case 1:
	  moveFish(i, k);
	  break;
	case 2:
	  moveShark(i, k);
	}//switch
      }//for k
    }//for i
    // barrier
#pragma omp barrier // everyone must complete their blocks!
    {
      // update top/bottom
      for (int k = rowBlocksStart; k < rowBlocksEnd; k+=tileRowSize-1){
	for (int i = columnBlocksStart; i < columnBlocksEnd; ++i){
	  switch(worldData[i][k].value){
	  case 1:
	    moveFish(i, k);
	    break;
	  case 2:
	    moveShark(i, k);
	  }//switch
	}//for i	
      }//for k
      // now update sides
      for (int i = columnBlocksStart; i < columnBlocksEnd; i+=columnBlocksEnd-1){
	for (int k = 1; k < rowBlocksEnd-1; ++k){
	  switch(worldData[i][k].value){
	  case 1:
	    moveFish(i, k);
	    break;
	  case 2:
	    moveShark(i, k);
	  }//switch	  
	}//for k
      }//for i      
    }//end barrier    
  }//end parallel
}//move

int main()
{
  // rng seed setup
  unsigned int seed = static_cast<unsigned int>(10);
  srand(seed);
  sf::Clock clock;

  NumThreads=omp_get_num_threads();
  
  std::cout << NumThreads;

  // populate all the sharks and fishes
  populate();
  // make a copy for restarts
  copyArray(worldData, restartData);
  
  for(int i=0;i<xdim;++i){
    for(int k=0;k<ydim;++k){//give each one a size, position and color
      recArray[i][k].setSize(sf::Vector2f(cellXSize,cellYSize));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);//position is top left corner!
      //int id=i*1-+k;      
      //put world data stuff here (if 0,1,2)
      if (worldData[i][k].value == 1)
	recArray[i][k].setFillColor(sf::Color::Green);
      else if (worldData[i][k].value == 2)
	recArray[i][k].setFillColor(sf::Color::Red);
      else
	recArray[i][k].setFillColor(sf::Color::Blue);
    }
  }
    sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");

    while (window.isOpen())
    {      
        sf::Event event;	
	
        while (window.pollEvent(event))
        {	  
	  if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
	    window.close();
	  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){//pause sim
	    paused = !paused;
	    if (paused)
	      std::cout << "Paused" << std::endl;
	    else
	      std::cout << "Unpaused" << std::endl;
	  }
	  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
	    copyArray(worldData, restartData);
	  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R){
	    std::cout << "Restarting..." << std::endl;
	    restart();
	    update();
	    std::cout << "Done!" << std::endl;
	  }
	  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P){
	    populate();
	    update();
	  }	  
        }
	//game loop here
	//updates everyones position
	if (clock.getElapsedTime().asSeconds() > updateDelay && !paused){
	  clock.restart();	  
	  move();
	  update();
	}
	
	//loop these three lines to draw frames
        window.clear(sf::Color::Black);
	for(int i=0;i<xdim;++i){
	  for(int k=0;k<ydim;++k){
	    window.draw(recArray[i][k]);
	  }
	}
        window.display();
    }// for -game/simulation loop

    return 0;
}

// 
// main.cpp ends here
