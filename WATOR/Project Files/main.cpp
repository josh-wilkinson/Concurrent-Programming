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

// window parameters
const unsigned int xdim = 50;
const unsigned int ydim = 50;
// dynamic parameters
const unsigned int fishBreed = 3;
const unsigned int sharkBreed = 6;
const unsigned int starve = 4;
//each shape will represent either a fish, shark or empty space
//e.g. blue for empty, red for shark and green for fish
sf::RectangleShape recArray[xdim][ydim];  
int worldData[xdim][ydim]; //contains a 0(nothing), 1(fish) or a 2(shark).
int WindowXSize=800;
int WindowYSize=600;
int cellXSize=WindowXSize/xdim;
int cellYSize=WindowYSize/ydim;
// ratio for initial fish/shark population
const float fishRatio = 0.01;
const float sharkRatio = 0.005;
//delay
const float updateDelay = 1;


int main()
{
  // rng seed setup
  unsigned int seed = static_cast<unsigned int>(10);
  srand(seed);
  // parameters
  int numShark = 0;
  int numFish = 0;
  int movePosition;
  int counter = 0;
  bool paused = false;
  sf::Clock clock;
  
  // seed initialisation (let's place the sharks and fishes)
  std::cout << "Placing sharks and fishes..." << std::endl;
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      int randomNumber = rand() % 3; // 0, 1, or 2.
      //std::cout << randomNumber;
      
      if (randomNumber == 1){
	worldData[i][k] = 1;
	numFish++;
      }
      else if (randomNumber == 2){
        worldData[i][k] = 2;
	numShark++;	
      }
      else{
	worldData[i][k] = 0;
      }     
    }
    //std::cout << std::endl;
  }
  
  
  for(int i=0;i<xdim;++i){
    for(int k=0;k<ydim;++k){//give each one a size, position and color
      recArray[i][k].setSize(sf::Vector2f(cellXSize,cellYSize));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);//position is top left corner!
      //int id=i*1-+k;
      
      //put world data stuff here (if 0,1,2)
      if (worldData[i][k]==1)
	recArray[i][k].setFillColor(sf::Color::Green);
      else if (worldData[i][k]==2)
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
	  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){}//pause sim
	  
        }
	//game loop here
	//updates everyones position
	if (clock.getElapsedTime().asSeconds() > updateDelay){
	  paused = true;
	  for (int i = 0; i < xdim; ++i){
	    for (int k = 0; k < ydim; ++k){
	      movePosition = rand() % 4; // generates number 0, 1, 2, or 3.
	      //std::cout << movePosition;
	      /*
		North: 0
		South: 1
		East: 2
		West: 3
	       */
	      switch (movePosition){
	      case 0: // go North y-1
		if (worldData[i][k] == 1){
		  if (k == 0){
		    if (worldData[i][ydim-1] < 1){
		      worldData[i][k] = 0;
		      worldData[i][ydim-1] = 1;
		    }
		  }
		  else{
		    if (worldData[i][k-1] < 1){
		      worldData[i][k] = 0;
		      worldData[i][k-1] = 1;
		    }
		  }
		}
		else if (worldData[i][k] == 2){
		  if (k == ydim-1){
		    if (worldData[i][ydim-1] != 2){
		      worldData[i][k] = 0;
		      worldData[i][ydim-1] = 2;
		    }
		  }
		  else{
		    if (worldData[i][k-1] != 2){
		      worldData[i][k] = 0;
		      worldData[i][k-1] = 2;
		    }
		  }
		}
		paused = false;		 
		break;
	      case 1: // go South y+1
		if (worldData[i][k] == 1){
		  if (k == ydim-1){
		    if (worldData[i][0] < 1){
		      worldData[i][k] = 0;
		      worldData[i][0] = 1;
		    }
		  }
		  else{
		    if (worldData[i][k+1] < 1){
		      worldData[i][k] = 0;
		      worldData[i][k+1] = 1;
		    }
		  }
		}
		else if (worldData[i][k] == 2){
		  if (k == ydim-1){
		    if (worldData[i][0] != 2){
		      worldData[i][k] = 0;
		      worldData[i][0] = 2;
		    }
		  }
		  else{
		    if (worldData[i][k+1] != 2){
		      worldData[i][k] = 0;
		      worldData[i][k+1] = 2;
		    }
		  }
		}
		paused = false;
		break;
	      case 2: // go East
		if (worldData[i][k] == 1){
		  if (i == (xdim-1)){
		    if (worldData[0][k] < 1){
		      worldData[i][k] = 0;
		      worldData[0][k] = 1;
		    }
		  }
		  else{
		    if (worldData[i+1][k] < 1){
		      worldData[i][k] = 0;
		      worldData[i+1][k] = 1;
		    }
		  }
		}
		else if (worldData[i][k] == 2){
		  if (i == (xdim-1)){
		    if (worldData[0][k] != 2){
		      worldData[i][k] = 0;
		      worldData[0][k] = 2;
		    }
		  }
		  else{
		    if (worldData[i+1][k] != 2){
		      worldData[i][k] = 0;
		      worldData[i+1][k] = 2;
		    }
		  }
		}
		paused = false;
		break;
	      case 3: // go West
		//std::cout << 'W';
		if(worldData[i][k] == 1){
		  if (i == 0){
		    if (worldData[xdim-1][k] < 1){
		      worldData[i][k] = 0;
		      worldData[xdim-1][k] = 1;
		    }
		  }
		  else{
		    if (worldData[i-1][k] < 1){
		      worldData[i][k] = 0;
		      worldData[i-1][k] = 1;
		    }
		  }
		}
		else if (worldData[i][k] == 2){
		  if (i == 0){
		    if (worldData[xdim-1][k] != 2){
		      worldData[i][k] = 0;
		      worldData[xdim-1][k] = 2;
		    }
		  }
		  else{
		    if (worldData[i-1][k] != 2){
		      worldData[i][k] = 0;
		      worldData[i-1][k] = 2;
		    }
		  }
		}
		paused = false;
		break;
	      }
	      
	      // update colours
	      if (worldData[i][k]==1)
		recArray[i][k].setFillColor(sf::Color::Green);
	      else if (worldData[i][k]==2)
		recArray[i][k].setFillColor(sf::Color::Red);
	      else
		recArray[i][k].setFillColor(sf::Color::Blue);

	      // let's slow it down a bit for testing
	      //sleep(0.1);
	      //counter += 1;
	      //std::cout << counter << std::endl;
	      clock.restart(); // restart the clock
	    
	    }
	  }
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
