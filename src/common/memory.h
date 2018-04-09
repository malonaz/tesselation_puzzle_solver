#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

using std::vector;

/*
  Note: because of the nature of how the compiler works, we need to place
        the implementation in this header file so that the templates will work.
 */

template<class T>
void cleanup_list(vector<T*> list) {
  while(!list.empty()) {
    T* ptr = list.back();
    list.pop_back();
    delete ptr;
  }
}

template<class T>
void cleanup_list(vector<T*>* list) {
  while(!list->empty()) {
    T* ptr = list->back();
    list->pop_back();
    delete ptr;
  }
  delete list;
}


#endif
