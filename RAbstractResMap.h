#ifndef RSRCLIB_RRESMAP_H
#define RSRCLIB_RRESMAP_H


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RResObject.h"


class RAbstractResMap : public RResObject {
public:
  RAbstractResMap();
  //
//protected:
  uint32_t pad0a,pad0b,pad0c,pad0d;  // NOTE: reserved for copy of resource header
  uint32_t pad1;  // NOTE: reserved for handle to next resource map to be searched
  uint16_t pad2;  // NOTE: reserved for file reference number
  uint16_t fileAttrib;  // ???
  uint16_t typeOffset;  // NOTE: From start of resource map to type list
  uint16_t nameOffset;  // NOTE: From start of resource map to resource name list
};


#endif
