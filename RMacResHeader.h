#ifndef RSRCLIB_RMACRESHEADER_H
#define RSRCLIB_RMACRESHEADER_H


// NOTE: These are Mac specific!


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RAbstractResHeader.h"


// For internal use...this is not an official Apple data structure


class RMacResHeader : public RAbstractResHeader {
public:
  RMacResHeader();
  //
//protected:
/*
  uint32_t dataOffset;  // NOTE: Should point to directly after this structure
  uint32_t mapOffset;
  uint32_t dataLen;
  uint32_t mapLen;
  // NOTE: Then 112 bytes reserved for system use here
  // NOTE: Then 128 bytes reserved for application data here
  //
  RMacResMap *macMap;
  RMacResEntry *macEnt;
  RMacResRef *macRef;
  */
};


#endif
