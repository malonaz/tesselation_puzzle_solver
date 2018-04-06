#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char** argv) {
  /* READ FILE */

  const char* hash = argv[1];
  const char* state = argv[2];

  const char* HASH_PROCESSING = "8bfae8c3c31548d76f137ce698aa9a02bdc478411e2b69d8502aaf63b984d8a4";
  const char* HASH_PROCESSED = "bb542fcff403f04507d4f4881e9a92c27ac21970dc54ea16b28aeda35971c65b";

  // cout << "Hash Passed in: " << hash << endl;
  // cout << "State Passed in: " << state << endl;
  // cout << "simulate searching for solution..." <<endl;

  unsigned int ms;
  ms = 5000;
  usleep(ms);

  if (!strcmp(hash, HASH_PROCESSED)){
    cout << "1 2 3 4" ;
    return 0;
  }
  else if (!strcmp(hash, HASH_PROCESSING)){
    cout << "-1" ;
    return 0;
  }
  else{
    cout << "-99";
    return 0;
  }
  return 0;
}
