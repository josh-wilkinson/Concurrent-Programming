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
#include <unistd.h> /* sleep() */
#include <iostream> /* for debugging */

int main()
{
  // rng seed setup
  unsigned int seed = static_cast<unsigned int>(10);
  srand(seed);
  
  int xdim = 100;
  int ydim= 100;
  int WindowXSize=800;
  int WindowYSize=600;
  int cellXSize=WindowXSize/xdim;
  int cellYSize=WindowYSize/ydim;

  int sharkCount = 0;
  int fishCount = 0;
  int sharkLimit = 80;
  int fishLimit = 80;
  
  //each shape will represent either a fish, shark or empty space
  //e.g. blue for empty, red for shark and green for fish
  sf::RectangleShape recArray[xdim][ydim];
  
  int worldData[xdim][ydim]; //contains a 0(nothing), 1(fish) or a 2(shark).
  
  // seed initialisation (let's place the sharks and fishes)
  std::cout << "Placing sharks and fishes..." << std::endl;
  for (int i = 0; i < xdim; ++i){
    for (int k = 0; k < ydim; ++k){
      int randomNumber = rand() % 3; // 0, 1, or 2.
      std::cout << randomNumber;
      
      if (randomNumber == 1){
	worldData[i][k] = 1;
	fishCount++;
      }
      else if (randomNumber == 2){
        worldData[i][k] = 2;
	sharkCount++;	
      }
      else{
	worldData[i][k] = 0;
      }     
    }
    std::cout << std::endl;
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
            if (event.type == sf::Event::Closed)
                window.close();
        }
	//game loop here
	//updates everyones position
	for (int i = 0; i < xdim; ++i){
	  for (int k = 0; k < ydim; ++k){
	    int direction;	    
	    switch (worldData[i][k]){
	    case 0:
	      // water detected
	      break;
	    case 1:
	      // fish detected!
	      direction = (rand()%4)+1; // 1(North), 2(South), 3(East), or 4(West).

	      if (direction == 1){
		if (i == 0){
		  
		}
		else{
		  worldData[i][k] = 0;
		  worldData[i-1][k] = 1;
		}
	      }
	      else if (direction == 2){
		if (i == xdim-1){

		}
		else{
		  worldData[i][k] = 0;
		  worldData[i+1][k] = 1;
		}
	      }
	      else if (direction == 3){
		if (k == ydim-1){

		}
		else{
		  worldData[i][k] = 0;
		  worldData[i][k+1] = 1;
		}
	      }
	      else{
		if (k == 0){

		}
		else{
		  worldData[i][k] = 0;
		  worldData[i][k-1] = 1;
		}
	      }
	      
	      break;
	    case 2:
	      // shark detected!
	      
	      
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
	    sleep(0.01);
	    
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
// test.cpp ends here
