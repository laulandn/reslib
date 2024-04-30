
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

#include "RAbstractResEntry.h"


////////////////////////////////////////////////////////////////////////////////
//  RAbstractResEntry Class
////////////////////////////////////////////////////////////////////////////////

RAbstractResEntry::RAbstractResEntry()
{
  type=0;
  num=0;
  refOffset=0;
}

