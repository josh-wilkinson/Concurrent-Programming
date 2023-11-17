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

/* Struct */
struct Square
{
  int value; // 0 = ocean, 1 = fish, 2 = shark.
  int sharkStarveProgress = 1;
  int age = 1;
};

/* Variables */

// window parameters
const int xdim = 50;
const int ydim = 50;
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
bool paused = false;
bool makeMoreFish = false;
bool makeMoreSharks = false;
// ratio for initial fish/shark population
const float fishRatio = 0.01;
const float sharkRatio = 0.005;
//delay
const float updateDelay = 1;

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
      copyArray[i][k] = array[i][k];
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

void move()
{
  fishCounter+=1; 
  sharkCounter+=1;
  makeMoreFish = false;
  makeMoreSharks = false;
  //std::cout << fishCounter;
  /*
  if (fishCounter == fishBreed){
    makeMoreFish = true;
    fishCounter = 0;
  }
  if (sharkCounter == sharkBreed){
    makeMoreSharks = true;
    sharkCounter = 0;
  }
  */
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      
      
      // now move fish or shark value into a random square
      movePosition = rand() % 4;
      moveFish(i, k);
    }
  }
}

int main()
{
  // rng seed setup
  unsigned int seed = static_cast<unsigned int>(10);
  srand(seed);
  sf::Clock clock;

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
	  //paused = false;	  
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
