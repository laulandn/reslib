
#ifdef BUILDING_NEMU
#define ERR_OUT nemuConsole
#define CONS_OUT nemuConsole
#define CONS_IN nemuConsole
#else
#define ERR_OUT std::cerr
#define CONS_OUT std::cout
#define CONS_IN std::cin
#endif
#define DEBUG_OUT std::cout


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include <stdio.h>
#include <stdlib.h>

#include "RPalmResHeader.h"


////////////////////////////////////////////////////////////////////////////////
//  RPalmResHeader Class
////////////////////////////////////////////////////////////////////////////////

RPalmResHeader::RPalmResHeader()
{
  for(unsigned int t;t<32;t++) { name[t]=0; }
  flags=0;
  version=0;
  //
  creationTime=0;
  modificationTime=0;
  backupTime=0;
  //
  mod_num=0;
  app_info=0;
  sort_info=0;
  type=0;
  creator=0;
  unique_id_seed=0;
  next_record_list=0;
  num_records=0;
}

