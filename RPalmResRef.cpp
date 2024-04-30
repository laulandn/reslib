
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

#include "RPalmResRef.h"


// NOTE: Some Palm docs call this "PRC Resource Header"...


////////////////////////////////////////////////////////////////////////////////
//  RPalmPrcResRef Class
////////////////////////////////////////////////////////////////////////////////

RPalmPrcResRef::RPalmPrcResRef()
{
  name=0;
  id=0;
  dataOffset=0;
}


////////////////////////////////////////////////////////////////////////////////
//  RPalmPdbResRef Class
////////////////////////////////////////////////////////////////////////////////

RPalmPdbResRef::RPalmPdbResRef()
{
  dataOffset=0;
  attributes=0;
}

