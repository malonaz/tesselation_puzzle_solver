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
  char* upload_dir = argv[2];

  cout << "File Passed in: " << image_file << endl;
  cout << "Upload Directory: " << upload_dir << endl;

  cout << "simulate solving..." <<endl;
  unsigned int ms;
  ms = 5000;
  usleep(ms);

  cout <<"solved" << endl;

  const char* name = "/solving";
  char* solving_file = strcat(upload_dir, name);
  cout << "file to remove: "<< solving_file <<endl;
  if( remove( solving_file ) != 0 ){
    cout << "Error deleting file" <<endl;
  }
  else{
    cout<< "File successfully deleted" <<endl;
  }

  return 0;
}
