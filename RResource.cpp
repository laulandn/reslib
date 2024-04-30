
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


#ifdef DEBUG_VERBOSE
#undef DEBUG_VERBOSE
#endif

// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#include <stdio.h>
#include <stdlib.h>

#include "../cpp/alib/adebug.h"

#include "RResource.h"


////////////////////////////////////////////////////////////////////////////////
//  RResource Class
////////////////////////////////////////////////////////////////////////////////

RResource::RResource(const char *tname,const char *ttype,unsigned int tnum,uint32_t toffset, unsigned int tsize)
{
  name=tname;
  type=ttype;
  num=tnum;
  offset=toffset;
  size=tsize;
}


RResource::~RResource()
{
}


void RResource::init()
{
  name=NULL;
  type=NULL;
  num=0;
  offset=0;
  size=0;
}


void RResource::debugDump()
{
  DEBUG_OUT<<"resource: \"";
  if(name) DEBUG_OUT<<name; else DEBUG_OUT<<"NULL";
  DEBUG_OUT<<"\" \""<<type<<"\" #"<<num<<" @"<<offset<<" "<<size<<" bytes.\n";
}

