#ifndef RSRCLIB_RRESOURCEFILE_H
#define RSRCLIB_RRESOURCEFILE_H


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


/*

A very braindead implementation of mac resource forks.

NOTE: This code has real trouble telling an actual resource file from just
any file.  Use caution when using this...
The error flag isn't used as well as it might to solve this...

*/


////////////////////////////////////////
//
//  Please note!  This is a hacky mess!
//
////////////////////////////////////////


// TODO: break this up into smaller parts, move code into other classes


#include <stdio.h>


#include "RResObject.h"
#include "RMacResHeader.h"
#include "RMacResMap.h"
#include "RMacResEntry.h"
#include "RMacResRef.h"


// NOTE: These are designed to be non-Mac specific, but not yet!


class AList_rsrc;


#define NEMU_ERR_NEMU_ERR_RESOURCEFILE "Resource file error!\n"


class RResFile : public RResObject {
public:
  //
  // Public member functions
  //
  RResFile(const char *name);
  virtual ~RResFile();
  //
  AList_rsrc *getResourceMap() { return resourceMap; }
  bool readFromNum(const char *type,unsigned int num,char *buffseter);
  bool readFromID(const char *type,unsigned int id,char *buffseter);
  unsigned int getSizeFromNum(const char *type,unsigned int num);
  unsigned int getSizeFromID(const char *type,unsigned int id);
  bool getNameFromNum(const char *type,unsigned int num,char *buf);
  bool getNameFromID(const char *type,unsigned int num,char *buf);
  unsigned int countType(const char *type);
  //
  void debugDump();
  //
protected:
  //
  // private member functions
  //
  virtual void init();
  //
  uint32_t tryToOpenFile(const char *name);
  bool checkSanity();
  //
  void printName(long offset);
  void printName(char *buf);
  //
  int32_t rdLong(long offset);
  int16_t rdShort(long offset);
  int8_t rdChar(long offset);
  uint32_t rdULong(long offset);
  uint16_t rdUShort(long offset);
  uint8_t rdUChar(long offset);
  //
  bool getRefFromNum(const char *type,unsigned int num);
  bool getRefFromID(const char *type,unsigned int id);
  const char *getCurType();
  unsigned int getCurCount();
  uint32_t getCurSize() const { return curSize; }
  long getDataOffset();
  unsigned int getDataSize();
  //
  bool readResEntry();
  bool readMacEntry();
  bool readPalmEntry();
  bool readResHeader();
  bool readMacHeader();
  bool readPalmHeader();
  //
  // Private data members
  //
  const char *filename;
  int fd;
  uint32_t binarySize;
  bool headerRead;
  //
  AList_rsrc *resourceMap;
  AList_rsrc *typeMap;
  RAbstractResHeader *resHeader;
  unsigned int numTypes;
  //
  long offset;  // NOTE: This should not be global to the object!
  //
  // Move the following to RResource object...even if only embeddded here!!!
  char ttype[5];  // NOTE: This should not be global to the object!
  unsigned int curNum;  // NOTE: This should not be global to the object!
  uint32_t curSize;  // NOTE: This should not be global to the object!
  //
};


#endif
