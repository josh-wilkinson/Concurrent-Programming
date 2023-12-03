// test.cpp --- 
// 
// Filename: test.cpp
// Description: 
// Author: Joseph
// Maintainer: Joshua Wilkinson
// Created: Fri Nov  3 15:51:15 2023 (+0000)
// Last-Updated: Fri Nov  3 16:49:06 2023 (+0000)
//           By: Joseph
//     Update #: 19
// 
// 

// Commentary: 
// 
// Using an SFML example made by Joseph to add the
// Wa-Tor simulation made by Joshua. 
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

/*! \fn Square
    \brief A struct that contains sharks, fish, sea, starve progress, age.
 */
struct Square
{
  int value; // 0 = ocean, 1 = fish, 2 = shark.
  int sharkStarveProgress = 0;
  int age = 0;
};

/* Variables */
const int NUMCORES=4; /*!< My core count -- change as required. */
const int NumThreads = NUMCORES * 2; /*!< number of threads used by OpenMP -- change as required. */
double itime; /*!< for time execution of OpenMP. */
double ftime; /*!< for time execution of OpenMP. */
double execTime; /*!< for time execution of OpenMP. */
drand48_data rng[NumThreads]; /*!< RNG container, one per thread. */
// window parameters
const int xdim = 200; /*!< Amount of rows. */
const int ydim = 200; /*!< Amount of columns. */
// dynamic parameters
const unsigned int fishBreed = 3; /*!< A fish can reproduce after (X) updates. */
const unsigned int sharkBreed = 6; /*!< A shark can reporduce after (X) updates. */
const int starve = 4; /*!< A shark will starve after (X) updates, unless it eats a fish. */
//each shape will represent either a fish, shark or empty space
//e.g. blue for empty, red for shark and green for fish
sf::RectangleShape recArray[xdim][ydim]; /*!< 2-D array of RectangleShape, used for the SFML display, takes x and y dimensions. */
Square worldData[xdim][ydim]; /*!< The 2-D array contains a a struct (Square) which contains the value 0(nothing), 1(fish) or a 2(shark). */
Square restartData[xdim][ydim]; /*!< A copy of the worldData for restarting the simulation. */
int WindowXSize=1000; /*!< Width of the window. */
int WindowYSize=1000; /*!< Height of the window. */
int cellXSize=WindowXSize/xdim; /*!< Row size for each square in the SFML window. */
int cellYSize=WindowYSize/ydim; /*!< Column size for each quare in the SFML window. */
int numShark;
int numFish;
bool paused = false; /*!< If the 'space' key is pressed, this will flip. TRUE will pause the sim. */
// ratio for initial fish/shark population
const float fishRatio = 0.05; /*!< Ratio for initial population of fish. */
const float sharkRatio = 0.005; /*!< Ratio for initial population of sharks. */
//delay
const float updateDelay = 0.1; /*!< Time in seconds for every update. */

/* Methods */
/*! \fn populate
  \brief This method is responsible for the initial population of the simulation,
  adding sharks and fish according the sharkRatio and fishRatio.
 */
void populate()
{
  numFish = 0;
  numShark = 0;
  std::cout << "Placing sharks and fishes..." << std::endl;
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){      
      float randomNumber = (rand() % 10000) / 10000.0; // random number between 0 and 1 (for fish or shark)      
      if (randomNumber <= fishRatio){
	// Increase the numFish
	worldData[i][k].value = 1;
	numFish++;
      }
      else if (randomNumber > fishRatio && randomNumber < fishRatio + sharkRatio){
	// Increase the numShark
        worldData[i][k].value = 2;
	numShark++;	
      }
      else{
	worldData[i][k].value = 0;
      }
      worldData[i][k].sharkStarveProgress = 0;
    }
  }
}
/*! \fn copyArray(Square array[xdim][ydim], Square copyArray[xdim][ydim])
  \brief Simply copies over the squares of one array to another.
 */
void copyArray(Square array[xdim][ydim], Square copyArray[xdim][ydim])
{
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      copyArray[i][k].value = array[i][k].value;
      copyArray[i][k].age = array[i][k].age;
      copyArray[i][k].sharkStarveProgress = array[i][k].sharkStarveProgress;
    }
  }
}
/*! \fn restart
  \brief Makes worldData equal to restartData.
 */
void restart()
{
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      worldData[i][k] = restartData[i][k];
    }
  }
}
/*! \fn update
  \brief Matches the worldData to the recArray, showing the position
  of the sharks and fish. Note: Red is Shark, Green is Fish, Blue is empty sea.
 */
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
/*! \fn moveFish(int x, int y)
  \brief Behaviour for moving a fish.
 */
void moveFish(int x, int y)
{
  // check for fish
  if (worldData[x][y].value != 1)
    return;
  // for storing empty square locations:
  int xFree[8] = {0};
  int yFree[8] = {0};

  int xIndex = 0;
  int yIndex = 0;
  // age for the square being left behind
  int oldPositionAge = 0;
  int newPositionAge = 0;
  
  int freeBlocksCounter = 0; // to count all the free adjacent blocks
  // check for adding a new fish
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
    long location;
    lrand48_r(&rng[omp_get_thread_num()], &location);
    location = location % freeBlocksCounter;
    int randomFreeBlockNumber = location; // get the index for xFree & yFree

    unsigned int xpos = xFree[randomFreeBlockNumber];
    unsigned int ypos = yFree[randomFreeBlockNumber];

    // now update worldData
    worldData[xpos][ypos].value = 1; // move fish
    if (breeding){
      worldData[x][y].value = 1;
      numFish++;
    }
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
/*! \fn moveShark(int x, int y)
  \brief Behaviour for moving a shark, similar to moveFish, except sharks
  can starve and must take out the any fish it comes across (randomly).
 */
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
    long location;
    lrand48_r(&rng[omp_get_thread_num()], &location);
    location = location % fishBlocksCounter;
    int randomFishBlockNumber = location; // get the index for xFree & yFree

    unsigned int xpos = xFish[randomFishBlockNumber];
    unsigned int ypos = yFish[randomFishBlockNumber];

    //std::cout << xpos << " " << ypos << std::endl;

    worldData[xpos][ypos].value = 0; // kill fish
    numFish--;

    // move shark
    worldData[xpos][ypos].value = 2;
    if (breeding){
      // new shark
      worldData[x][y].value = 2;
      worldData[x][y].sharkStarveProgress = 1;
      numShark++;
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
    long location;
    lrand48_r(&rng[omp_get_thread_num()], &location);
    location = location % freeBlocksCounter;
    int randomFreeBlockNumber = location; // get the index for xFree & yFree

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
      numShark++;
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
      numShark--;
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
      numShark--;
    }
  }
  
}  

/*! \fn move
  \brief This method is responsible for moving the squares (shark, fish),
  but also parallelising the movements with OpenMP.
 */
void move()
{
  // First thing, declare the parallel region...
#pragma omp parallel num_threads(NumThreads)
  {//parallel start
    // first start the execution timer
    itime = omp_get_wtime();
    // now declare the variables for the tiling
    int tid = omp_get_thread_num();
    int tileRowSize = (xdim/omp_get_num_threads());    
    int rowBlocksStart = tid * tileRowSize;    
    int columnBlocksStart = 0;    
    int rowBlocksEnd = rowBlocksStart + tileRowSize;
    int columnBlocksEnd = ydim;
    // failsafes
    if (rowBlocksEnd > xdim)
      rowBlocksEnd = xdim;
    if (columnBlocksEnd > ydim)
      columnBlocksEnd = ydim;
    if ((omp_get_num_threads() == omp_get_thread_num()+1) && rowBlocksEnd < xdim){
      rowBlocksEnd = xdim + 1;
    }
    // update movement for inner blocks
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
    // barrier beginning
#pragma omp barrier // everyone must complete their blocks!
    {
      // update movement for outer blocks
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
}// end move
/*! \fn main
  \brief Driver containing the game loop and some initialisations.
  It starts the window and detects inputs. Note: space for pause, 
  'R' for restart, 'P' for populate(random restart), 'C' for copy, Escape for quit.
 */
int main()
{
  // rng seed setup
  unsigned int seed = static_cast<unsigned int>(10);
  srand48(seed);
  sf::Clock clock;
  bool gotSpeedupTime = false;
  int speedupCounter = 0;

  // populate all the sharks and fishes
  populate();
  // make a copy for restarts
  copyArray(worldData, restartData);
  
  for(int i=0;i<xdim;++i){
    for(int k=0;k<ydim;++k){//give each one a size, position and color
      recArray[i][k].setSize(sf::Vector2f(cellXSize,cellYSize));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);//position is top left corner!
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
    // let's start the window...
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
	  itime = omp_get_wtime();
	  move();
	  ftime = omp_get_wtime();
	  if (!gotSpeedupTime){
	    execTime = ftime - itime;
	    gotSpeedupTime = true;
	    std::cout << "num shark: " << numShark << std::endl;
	    std::cout << "num fish: " << numFish << std::endl;
	  }
	  update();//sfml
	  speedupCounter++;
	  //simpletimer::SimpleTimer::get().printAllResults();
	}
	
	//loop these three lines to draw frames
        window.clear(sf::Color::Black);
	for(int i=0;i<xdim;++i){
	  for(int k=0;k<ydim;++k){
	    window.draw(recArray[i][k]);
	  }
	}
        window.display();
	if (speedupCounter > 100 && !gotSpeedupTime){
	  //simpletimer::SimpleTimer::get().stop("Speedup Timer");
	  //simpletimer::SimpleTimer::get().printAllResults();
	  speedupCounter = 0;
	  gotSpeedupTime = true;
	}
    }// for -game/simulation loop
    std::cout << "Execution Time: " << execTime << std::endl;
    std::cout << "Grid size: " << xdim << " x " << ydim << std::endl;
    std::cout << "num threads: " << NumThreads << std::endl;
    return 0;
}

// 
// main.cpp ends here
