#include "utils.h"

#include <iostream>
#include <string>


using std::string;
using std::cerr;
using std::endl;

bool valid_args(int argc, set<int> expected_args, string correct_format){
  
  // check number of arguments 
  if (expected_args.find(argc - 1) != expected_args.end())
    return true;

  // compute grammar of arguments
  string argument = argc == 1? " argument": " arguments";

  // print msg to std error stream
  cerr << "Error: " << argc << argument << " provided. ";
  cerr << "Correct format: " << correct_format << endl;

  return false;
}
