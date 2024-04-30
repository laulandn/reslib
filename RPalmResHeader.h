#ifndef RSRCLIB_RPALMRESHEADER_H
#define RSRCLIB_RPALMRESHEADER_H


// NOTE: These are Palm specific!


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include "RAbstractResHeader.h"

#include "RPalmResRef.h"


// For internal use...this is not an official Palm data structure


class RPalmResHeader : public RAbstractResHeader {
public:
  RPalmResHeader();
  //
//protected:
  char name[32];
  uint16_t flags;
  uint16_t version;
  //
  uint32_t creationTime;
  uint32_t modificationTime;
  uint32_t backupTime;
  //
  uint32_t mod_num;
  uint32_t app_info;
  uint32_t sort_info;
  uint32_t type;
  uint32_t creator;
  uint32_t unique_id_seed;
  uint32_t next_record_list;
  uint16_t num_records;
};


#endif
