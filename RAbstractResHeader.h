#ifndef RSRCLIB_RRESHEADER_H
#define RSRCLIB_RRESHEADER_H


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RResObject.h"

#include "RAbstractResMap.h"
#include "RAbstractResEntry.h"
#include "RAbstractResRef.h"


class RAbstractResHeader : public RResObject {
public:
  RAbstractResHeader();
  //
//protected:
  uint32_t dataOffset;
  uint32_t mapOffset;
  uint32_t dataLen;
  uint32_t mapLen;
  //
  RAbstractResMap *resMap;
  RAbstractResEntry *resEnt;
  RAbstractResRef *resRef;
};


#endif
