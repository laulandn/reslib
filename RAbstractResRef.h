#ifndef RSRCLIB_RRESREF_H
#define RSRCLIB_RRESREF_H


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RResObject.h"


class RAbstractResRef : public RResObject {
public:
  RAbstractResRef();
  //
//protected:
  uint16_t id;
  uint16_t nameOffset;
  char attrib;
  uint32_t dataOffset;
};


#endif
