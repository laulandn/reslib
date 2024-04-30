#ifndef RSRCLIB_RMACRESREF_H
#define RSRCLIB_RMACRESREF_H


// NOTE: These are Mac specific!


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RAbstractResRef.h"


// For internal use...this is not an official Apple data structure


class RMacResRef : public RAbstractResRef {
public:
  RMacResRef();
  //
//protected:
/*
  uint16_t id;
  uint16_t nameOffset;
  char attrib;
  */
  char pad0;
  /*
  uint32_t dataOffset; // NOTE: Actually 3 bytes in file
  */
  uint32_t pad1;
};


#endif
