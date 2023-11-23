/* SafeBuffer.h --- 
 * 
 * Filename: SafeBuffer.h
 * Description: This file contains the headers for SafeBuffer.cpp
 * Author: Joseph
 * Maintainer: Joshua Wilkinson
 * Created: Tue Jan  8 12:30:23 2019 (+0000)
 * Version: 1
 * Package-Requires: ()
 * Last-Updated: Tue Jan  8 12:30:25 2019 (+0000)
 *           By: Joseph
 *     Update #: 1
 * URL: https://github.com/josh-wilkinson/Concurrent-Programming/blob/master/Lab%204/SafeBuffer.h
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * Came to the conclusion that there is no need to make this class templated, since we're only using Event objects
 * 
 */

/* Change Log:
 *
 * Added headers to be used in SafeBuffer.cpp
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

/*! \class SafeBuffer
    \brief A thread safe implementation of a buffer
*/

#include "Semaphore.h"
#include <vector>
#include "Event.h"

class SafeBuffer{
private:
  std::vector<Event> items; /**< contains all the Event objects */
  std::shared_ptr<Semaphore> theMutex;
  std::shared_ptr<Semaphore> semaphore;
public:
  SafeBuffer();
  void push(Event theEvent);
  Event pop();
};

/* SafeBuffer.h ends here */


