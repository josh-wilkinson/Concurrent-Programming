#include <memory>

//-------------------------------------------------------------------
// Buffer (Class)
//     The buffer is implemented with a templated class so
//     the buffer can be a buffer of specified type.
//-------------------------------------------------------------------
template <class T> class Buffer{
private:
  //---------------------------------------------------------------
  // Buffer - Private Member Variables
  //---------------------------------------------------------------

  std::unique_ptr<T[]> buffer; // using a smart pointer is safer (no need to implement a destructor)
  size_t head = 0;             // size_t is an unsigned long
  size_t tail = 0;
  size_t max_size;
  T empty_item; // used to clear data
public:
  //---------------------------------------------------------------
  // Buffer - Public Methods
  //---------------------------------------------------------------

  // Create a new Buffer.
  Buffer<T>(size_t max_size)
      : buffer(std::unique_ptr<T[]>(new T[max_size])), max_size(max_size){};

  // Add an item to this buffer.
  void enqueue(T item){
    // if buffer is full, throw an error
    if (is_full())
      throw std::runtime_error("buffer is full");

    // insert item at back of buffer
    buffer[tail] = item;

    // increment tail
    tail = (tail + 1) % max_size;
  }

  // Remove an item from this buffer and return it.
  T dequeue(){
    // if buffer is empty, throw an error
    if (is_empty())
      throw std::runtime_error("buffer is empty");

    // get item at head
    T item = buffer[head];
    // set item at head to be empty
    T empty;
    buffer[head] = empty_item;
    // move head foward
    head = (head + 1) % max_size;

    // return item
    return item;
  }

  // Return the item at the front of this buffer.
  T front(){
    return buffer[head];
  }

  // Return true if this buffer is empty, and false otherwise.
  bool is_empty(){
    return head == tail;
  }

  // Return true if this buffer is full, and false otherwise.
  bool is_full(){
    return tail == (head - 1) % max_size;
  }

  // Return the size of this buffer.
  size_t size(){
    if (tail >= head)
      return tail - head;
    return max_size - head - tail;
  }
};
