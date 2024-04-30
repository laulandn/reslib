#ifndef RSRCLIB_RRESOBJECT_H
#define RSRCLIB_RRESOBJECT_H


// NOTE: These are designed to be non-Mac specific, but aren't quite yet!


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "Rabasics.h"


#include <stdint.h>


//#define RSRC_CRAZY_DEBUG 1


// A parent class to consolidate all the resource related classes


class RResObject
{
public:
  RResObject();
  //
  AError_rsrc err;
  //
//protected:
};


#endif
