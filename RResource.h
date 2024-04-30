#ifndef RSRCLIB_RRESOURCE_H
#define RSRCLIB_RRESOURCE_H


// NOTE: These are designed to be non-Mac specific, but not yet!


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RResObject.h"


// platform-independant (i.e. not Mac) resource
// this is what resources are stored as in the map


#define NEMU_ERR_RESOURCE "Resource error!\n"


class RResource : public RResObject {
public:
  RResource(const char *tname,const char *ttype,unsigned int tnum,uint32_t toffset,unsigned int tsize);
  virtual ~RResource();
  //
//protected:
  //
  virtual void init();
  void debugDump();
  //
  const char *name;
  const char *type;
  unsigned int num;
  uint32_t offset;
  unsigned int size;
};


#endif
