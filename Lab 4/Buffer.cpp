#include <memory>

template <class T> class Buffer
{
public:
  struct Node{
    int data;
    struct Node* next;
    struct Node* prev;
  };

  // method to insert at the end
  void insertEnd(struct Node** start, int value){
    // If list is empty, create a single node.
    if (*start == NULL){
      struct Node* newNode = new Node;
      newNode->data = value;
      *start = newNode;
      return;
    }
    // If list is not empty
    Node* last = (*start)->prev;
    struct Node* newNode = new Node;
    newNode->data = value;
    newNode->next = *start;
    (*start)->prev = newNode;
    newNode->prev = last;
    last->next = newNode;    
  }

}
