
#define DEBUG_OUT std::cout
#define ERR_OUT std::cerr
#define CONS_IN std::cin
#define CONS_OUT std::cout


// Open and display contents of a Mac OS classic resource file


#ifdef __linux__
#define DEFAULT_FILE "/home/nick/src/rsrc/macres/MacRsrc.rsrc"
#endif // __linux__

#ifdef __APPLE__
#define DEFAULT_FILE "/Users/nick/src/rsrc/macres/MacRsrc.rsrc"
#endif // __APPLE__


#include <iostream>

#include "RResFile.h"


const char *theFilename=NULL;


int main(int argc, char **argv)
{
  if(argc!=2) {
#ifdef DEFAULT_FILE
    theFilename=(char *)DEFAULT_FILE;
#else
    std::cout<<argv[0]<<": Usage: "<<argv[0]<<" mac_resource_file\n";
    return 0;
#endif // DEFAULT_FILE
  }
  else {
    theFilename=argv[1];
  }
  if(!theFilename) { std::cout<<argv[0]<<": Usage: "<<argv[0]<<" mac_resource_file\n"; return 0; }
  //
  RResFile *myRsrcFile=new RResFile(theFilename);
  if(!myRsrcFile) { std::cout<<""<<argv[0]<<": couldn't get myRsrcFile!\n"; return 0; }
  if(myRsrcFile->err.getError()) { std::cout<<"An error occured reading \""<<argv[0]<<"\"!\n"; return 0; }
  //
  myRsrcFile->debugDump();
  //
  return 0;
}
