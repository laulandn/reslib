#ifndef RSRCLIB_RMACRESENTRY_H
#define RSRCLIB_RMACRESENTRY_H


// NOTE: These are Mac specific!


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RAbstractResEntry.h"


// For internal use...this is not an official Apple data structure


class RMacResEntry : public RAbstractResEntry {
public:
  RMacResEntry();
  //
//protected:
/*
  uint32_t type;
  uint16_t num;
  uint16_t refOffset;
  */
};


#endif
