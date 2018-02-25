#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

using namespace std;

 void extract_dir(const char* filename, char* dir) {
   int length =0;
   while(filename[length] != '\0'){
     length++;
   }
   int idx =0;
   for (; idx<length-9; idx++ ){
     dir[idx] = filename[idx];
   }
   dir[length-9] = '\0';
}

int main(int argc, char** argv) { //  ./demo <input_filename>
  /* READ FILE */

  const char* image_file = argv[1];
  const char* hash = argv[2];

  cout << "File Passed in: " << image_file << endl;
  cout << "File Hash Passed in: " << hash << endl;

  cout << "simulate solving..." <<endl;
  unsigned int ms;
  ms = 5000;
  usleep(ms);

  cout <<"solved" << endl;

  // now will try to delete the solving file...
  // char upload_dir[512];
  // extract_dir(image_file, upload_dir);
  char upload_dir[512] = "./../web/puzzles/uploads/";
  char* file_dir = strcat(upload_dir, hash);
  char* name = "/solving";
  char* solving_file = strcat(file_dir, name);
  cout << "file to remove: "<< solving_file <<endl;
  if( remove( solving_file ) != 0 ){
    cout << "Error deleting file" <<endl;
  }
  else{
    cout<< "File successfully deleted" <<endl;
  }

  return 0;
}
