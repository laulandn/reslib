#ifndef RSRCLIB_RPALMRESREF_H
#define RSRCLIB_RPALMRESREF_H


// NOTE: These are Palm specific!


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RAbstractResRef.h"


// For internal use...this is not an official Palm data structure


// NOTE: Some Palm docs call this "PRC Resource Header"...


class RPalmPrcResRef : public RAbstractResRef {
public:
  RPalmPrcResRef();
  //
//protected:
  uint32_t name;
};


class RPalmPdbResRef : public RAbstractResRef {
public:
  RPalmPdbResRef();
  //
//protected:
  uint8_t attributes;
};


#endif
