/************************************************************************

    File: list.cc
    Author: Steven Cunden
    Date: October 26th, 2017

    Implementation of a doubly linked list data structure

************************************************************************/

#include "list.h"

// List node constructors provided by Prof. Campbell

list_node::list_node() : prev(NULL), next(NULL) {}

list_node::list_node(list_node *p, list_node *n) : prev(p), next(n) {}

list_node::list_node(list_node *p, const list_element & d, list_node * n):
  prev(p), data(d), next(n) {}

// List constructors/destructor/assignment also provided.  
// No need to change these.

list::list() 
{
  _init();
}

list::~list()
{
  _destroy();
}

list::list(const list & orig)
{
  _copy(orig);
}

list & list::operator=(const list & rhs)
{
  if (this != &rhs) {
    _destroy();
    _copy(rhs);
  }
  return *this;
}

void list::_init()
{ // Initializing front, rear, current index and size
  
  _front = new list_node(NULL, NULL);
  _rear = new list_node(_front, NULL);
  
  _front->next = _rear;
  _current = _rear;

  _current_index = 0;
  _size = 0; 
}

void list::_copy(const list & orig)
{ // Method makes deep copy of original list
  
  // Initializes new list
  _init();
  
  // Makes a deep copy of orig
  for (size_t i = 0; i < orig.size(); i++)
    add(orig.get(i),i);
}

void list::_destroy()
{ // Removes everything in list
  
  // Removes first item until list is empty
  while (size())
    remove_at(0);
  
  // Delte instance variables
  delete _front;
  delete _rear;
}

void list::add(const list_element & item, int index)
{ // Adds new item in list and correctly relinks
  
  // If index is negative, count index from rear
  if (index < 0)
    index += size();
  
  // Converts index into size_t
  size_t idx = index;
  _set_current_index(idx);

  // Re links the added node
  list_node * baby = new list_node(_current->prev, item, _current);
  _current -> prev ->next = baby;
  _current -> prev = baby;
  
  _current = baby;
  // Increases the size
  _size++;
}

void list::remove_at(int index)
{ // Removes the node at the given index

  // If index is negative, count index from rear
  if (index < 0)
    index += size();

  // Converts index into size_t
  size_t idx = index;
  _set_current_index(idx);

  // Re arranges the nodes prior to deleting
  _current -> prev -> next = _current -> next;
  _current -> next -> prev = _current -> prev;

  // Deletes the node at index
  list_node * baby = _current;
  _current=_current->next;
  delete baby;

  // Decrease the size
  _size--;
}

void list::remove(const list_element & item)
{ // Remove the node with item from the list

  for(size_t i = 0; i < _size; i++) 
    if (get(i) == item)
      remove_at(i);
}

size_t list::find(const list_element & item) const
{ // return smallest non-negative integer where get(i)==item

  for(size_t i = 0; i < _size; i++) 
    if (get(i) == item)
      return i;

  // or size() if no such integer
  return size();
}

list_element list::get(int index) const
{ // Gets the data from the node at given index

  // If index is negative, count index from rear
  if (index < 0)
    index += size();

  // Converts index into size_t
  size_t idx = index;
  _set_current_index(idx);

  // Returns data in current
  return _current -> data;
}

size_t list::size() const
{ // Returns size of list
  return _size;
}

void list::output(std::ostream & ostr) const
{ // Outputs all the elements of the list
  // in a < .... > fashion

  ostr << "<";

  for(size_t i = 0; i < _size; i++) {

    ostr << get(i);

    // If there's a next node, add a comma
    if (i < _size-1)
      ostr << ", ";
  }
  ostr << ">"; 
}

void list::_set_current_index(size_t index) const
{ // Set current pointing to the node at given index

  // Creates variable dir for direction
  size_t dir;
  
  // Checks for constant time cases
  if (index == 0 or index == _size or index == _current_index) {
    // Current is the first node
    if (index == 0) {
      _current = _front -> next;
      _current_index = 0;
      return;
    }

    // Current is the last node
    else if (index == _size) {
      _current = _rear;
      _current_index = _size;
      return;
    }
    // Current is already where it needs to be
    return;
  }  
  // Non constant time cases

  // If index is less than current index

  else if (index < _current_index) {
    // Start from index and go down
    if (_current_index - index < index) 
      dir = -1;

    // Start from top of queue and go up
    else {
      dir = 1;
      _current_index = 0;
      _current = _front -> next;
    }
  }

  // If index is greater than current index
  else {
    // Start from current index and go up
    if (index - _current_index < _size - index) 
      dir = 1;

    else {
      // Start from back of queue and go down
      dir = -1;
      _current_index = _size;
      _current = _rear;
    }
  }

  // Go through the nodes, starting from current node
  for (size_t i = _current_index; i!= index; i+=dir) {

    // If direction is increasing, advance towards back of queue
    if (dir == 1) {
      _current = _current->next;
      _current_index++;
    }

    // Else advance towards front of queue
    else {
      _current = _current->prev;
      _current_index--;
    }
  }

}