#ifndef UTILS_H
#define UTILS_H

#include <string>

using std::string;

/**
 * Returns true if arguments are correct. Returns false otherwise and 
 * prints an error message to standard error stream.
 */
bool valid_args(int argc, int expected_argc, string correct_format);


#endif
