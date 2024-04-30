#ifndef RSRCLIB_RRESENTRY_H
#define RSRCLIB_RRESENTRY_H


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RResObject.h"


class RAbstractResEntry : public RResObject {
public:
  RAbstractResEntry();
  //
//protected:
  uint32_t type;
  uint16_t num;
  uint16_t refOffset;
};


#endif
