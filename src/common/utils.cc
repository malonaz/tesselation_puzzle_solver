#include "utils.h"

#include <iostream>
#include <string>


using std::string;
using std::cerr;
using std::endl;

bool valid_args(int argc, int expected_argc, string correct_format){

  // check number of arguments 
  if (argc - 1 == expected_argc)
    return true;

  // compute grammar of arguments
  string argument = argc == 1? " argument": " arguments";

  // print msg to std error stream
  cerr << "Error: " << argc << argument << " provided. ";
  cerr << "Correct format: " << correct_format << endl;

  return false;
}
