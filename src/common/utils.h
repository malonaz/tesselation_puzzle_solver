#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <set>

using std::string;
using std::set;

/**
 * Returns true if arguments are correct. Returns false otherwise and 
 * prints an error message to standard error stream.
 *  @params:
 *   argc: the number arguments your main function received, including itself
 *   expected_args: a set of the valid argument numbers
 *   correct_format: a string corresponsing to the correct way to call your function 
 */ 
bool valid_args(int argc, set<int> expected_args, string correct_format);


#endif
