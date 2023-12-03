# README
This is an implementation of the Wa-Tor, population dynamics simulation.
<p float="left">
<a><img src="https://img.shields.io/badge/Language-C++-blue"></a>
<a><img src="https://img.shields.io/badge/version-1.0-red"></a>
</p>
***
## More Info:
***
### Name
Joshua Wilkinson
### Install
C++ Compiler (e.g. g++) SFML
### License:
GPL-3.0
***
## Sample Output
***
![alt text](https://github.com/josh-wilkinson/Concurrent-Programming/blob/master/WATOR/Project%20Files/Screenshot.png "Title")
Every fish and shark has an age and a position in the worldData 2-D array. Also, every shark has a starvation
progress. Each fish and shark is placed randomly according to a ratio. The rules are:
1. A fish can move into a random empty square if available, or it does not move.
2. A shark is the same, but if it finds a fish it eats it and resets its starvation progress. Otherwise, it increases by one.
3. After each update, the age of each shark and fish is increased.
4. If the age of a shark/fish is equal to sharkBreed/fishBreed, a new shark/fish is created in its previous place.
5. If a shark cannot find a fish in time, it starves and is removed from the worldData.
***
### Running the Code
***
1. Type 'make -k' in the terminal opened in the project folder.
2. Then type './WaTor' to run the executable.
