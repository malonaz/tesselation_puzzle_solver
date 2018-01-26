#include <vector>

using std::vector;

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
