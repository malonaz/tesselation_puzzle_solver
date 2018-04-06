#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char** argv) { //  ./demo <input_filename>
  /* READ FILE */

  const char* image_file = argv[1];
  const char* upload_dir = argv[2];

  int length =0;
  while(upload_dir[length] != '\0'){
    length++;
  }

  cout << "File Passed in: " << image_file << endl;
  cout << "Upload Directory: " << upload_dir << endl;

  cout << "simulate processing..." <<endl;
  unsigned int ms;
  ms = 5000;
  usleep(ms);

  cout <<"processed" << endl;

  const char* name = "/processing";
  char* processing_file = new char[length + 1];
  strcpy(processing_file, upload_dir);
  processing_file = strcat(processing_file, name);
  cout << "file to remove: "<< processing_file <<endl;
  if( remove( processing_file ) != 0 ){
    cout << "Error deleting file" <<endl;
    return 0;
  }
  else{
    cout<< "File successfully deleted" <<endl;
    const char* pieces = "/pieces";
    char* pieces_file = new char[length + 1];
    strcpy(pieces_file, upload_dir);
    pieces_file = strcat(pieces_file, pieces);
    ofstream out;
    out.open(pieces_file);
    if (!out.good()){
      cout << pieces_file <<endl;
      cout <<"Error creating pieces file..."<<endl;
      return 0;
    }
    out.put('\0');
    out.close();
    cout << "pieces file created"<<endl;
  }
  return 0;
}
