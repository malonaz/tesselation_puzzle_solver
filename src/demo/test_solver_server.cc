#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

using namespace std;

 void extractHash(const char* filename, char* hash) {
   int length = 64; //sha256 hash length
   for (int i =0; i<length; i++ ){
     hash[i] = filename[i];
   }
   hash[length] = '\0';
}

int main(int argc, char** argv) { //  ./demo <input_filename>
  /* READ FILE */

  const char* image_file = argv[1];

  cout << "File Passed in: " << image_file << endl;

  cout << "simulate solving..." <<endl;
  unsigned int ms;
  ms = 5000;
  usleep(ms);

  cout <<"solved" << endl;

  // now will try to delete the solving file...
  char* upload_dir = "./../../../web/puzzles/uploads/";
  char hash[65];
  extractHash(image_file, hash);
  char* file_dir = strcat(upload_dir, hash);
  char* name = "/solving";
  char* solving_file = strcat(file_dir, name);
  if( remove( solving_file ) != 0 ){
    cout << "Error deleting file" <<endl;
  }
  else{
    cout<< "File successfully deleted" <<endl;
  }

  return 0;
}
