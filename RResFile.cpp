
#ifdef BUILDING_NEMU
#define ERR_OUT nemuConsole
#define CONS_OUT nemuConsole
#define CONS_IN nemuConsole
#else
#define ERR_OUT std::cerr
#define CONS_OUT std::cerr
#define CONS_IN std::cin
#endif
#define DEBUG_OUT std::cerr


#ifdef DEBUG_VERBOSE
#undef DEBUG_VERBOSE
#endif


////////////////////////////////////////
//
//  Please note!  This is a hacky mess!
//
////////////////////////////////////////


// NOTE: This version of the file is not dependant on any other libs such as nemu/elib


#define ALIB_HAVE_FCNTL_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../cpp/alib/adebug.h"

#include "RResFile.h"
#include "RResource.h"

#include "Rabasics.h"


#ifdef ALIB_HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif // ALIB_HAVE_SYS_FCNTL_H
#ifdef ALIB_HAVE_FCNTL_H
#include <fcntl.h>
#endif // ALIB_HAVE_FCNTL_H


#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef O_RDONLY
#define O_RDONLY 0
#endif

#ifndef O_RSRC
#define O_RSRC 0
#endif


// arbitrary max filename length
#define DDISK_MAX_NAMELEN 256


////////////////////////////////////////////////////////////////////////////////
//  RResFile Class
////////////////////////////////////////////////////////////////////////////////

RResFile::RResFile(const char *name)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"RResFile(\""<<name<<"\")...\n";
#endif
  init();
  if(!name) {
    ERR_OUT<<"RResFile no name.\n";
    err.setError(); return;
  }
  char rffilename[DDISK_MAX_NAMELEN];
  rffilename[0]=0;
  filename=strdup(name);
  //
  //
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to try plain regular file/path...\n";
#endif
  binarySize=tryToOpenFile(filename);
  //
  //
  if(!binarySize) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"File size was zero, not regular file/path...\n";
#endif
    const char *sfilename=filename; //thedosym->getShortName(filename);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"The short name is \""<<sfilename<<"\"\n";
#endif
    AStringHelper_rsrc::fillInPathFromFullName(rffilename,filename);  // TODO: Is this wrong?
    if(rffilename[0]) AStringHelper_rsrc::fillInString(rffilename,"/");  // TODO: Is this wrong?
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Building full path with path \""<<rffilename<<"\"\n";
#endif
    AStringHelper_rsrc::fillInString(rffilename,"resource.frk/");  // TODO: Is this wrong?
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Building full path with \"resource.frk/\" \""<<sfilename<<"\"\n";
#endif
    AStringHelper_rsrc::fillInNameFromFullName(rffilename,sfilename);  // TODO: Is this wrong?
    binarySize=tryToOpenFile(rffilename);
  }
  //
  //
  if(!binarySize) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"File size was zero, going to try lowercasing the name...\n";
#endif
    // Try a lowercase version...
    for(unsigned int t=0;t<DDISK_MAX_NAMELEN;t++) {
      if((rffilename[t]>='A')&&(rffilename[t]<='Z')) rffilename[t]+='a'-'A';
    }
    binarySize=tryToOpenFile(rffilename);
  }
  //
  //
  if(!binarySize) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"File size was zero, going to try look in .resource folder...\n";
#endif
    // Now look in a Linux HFS style .resource folder
    // TODO: Actually check .resource exists!
    rffilename[0]=0;
    AStringHelper_rsrc::fillInPathFromFullName(rffilename,filename);  // TODO: Is this wrong?
    if(rffilename[0]) AStringHelper_rsrc::fillInString(rffilename,"/");  // TODO: Is this wrong?
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Building full path with path \""<<rffilename<<"\"\n";
#endif
    AStringHelper_rsrc::fillInString(rffilename,".resource/");  // TODO: Is this wrong?
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Building full path with \".resource/ \""<<rffilename<<"\"\n";
#endif
    AStringHelper_rsrc::fillInNameFromFullName(rffilename,filename);  // TODO: Is this wrong?
    binarySize=tryToOpenFile(rffilename);
  }
  //
  if(!binarySize) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"File size was zero, going to try adding ._ to filename...\n";
#endif
    // Now look for MacOSX style ._ resource fork...
    rffilename[0]=0;
    AStringHelper_rsrc::fillInPathFromFullName(rffilename,filename);  // TODO: Is this wrong?
    if(rffilename[0]) AStringHelper_rsrc::fillInString(rffilename,"/");  // TODO: Is this wrong?
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Building full path with path \""<<rffilename<<"\"\n";
#endif
    AStringHelper_rsrc::fillInString(rffilename,"._");  // TODO: Is this wrong?
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Building full path with \"._ \""<<rffilename<<"\"\n";
#endif
    AStringHelper_rsrc::fillInNameFromFullName(rffilename,filename);  // TODO: Is this wrong?
    binarySize=tryToOpenFile(rffilename);
  }
  //
  //
  if(!binarySize) {
    ERR_OUT<<"RResFile couldn't find a valid file!\n";
    err.setError(); return;
  }
  if(readResHeader()) {
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"Looks like readResHeader() was good.\n";
#endif
  }
  else {
    ERR_OUT<<"readResHeader() failed! Probably not a resource file!\n";
    err.setError();
  }
}


void RResFile::init()
{
  filename=NULL;
  fd=0;
  binarySize=0;
  offset=0;
  curNum=0;
  headerRead=false;
  resourceMap=new AList_rsrc;
  typeMap=new AList_rsrc;
  resHeader=new RAbstractResHeader;
  resHeader->resMap=new RAbstractResMap;
  resHeader->resEnt=new RAbstractResEntry;
  resHeader->resRef=new RAbstractResRef;
}


RResFile::~RResFile()
{
  if(!err.getError()) {
    if(fd) close(fd);
    //if(filename) free(filename);
    if(resourceMap) delete resourceMap;
    resourceMap=(AList_rsrc *)NULL;
    if(typeMap) delete resourceMap;
    typeMap=(AList_rsrc *)NULL;
    if(resHeader->resMap) delete resHeader->resMap;
    resHeader->resMap=(RAbstractResMap *)NULL;
    if(resHeader->resEnt) delete resHeader->resEnt;
    resHeader->resEnt=(RAbstractResEntry *)NULL;
    if(resHeader->resRef) delete resHeader->resRef;
    resHeader->resRef=(RAbstractResRef *)NULL;
    if(resHeader) delete resHeader;
    resHeader=(RAbstractResHeader *)NULL;
 }
}


// Returns file size if successful...
uint32_t RResFile::tryToOpenFile(const char *name)
{
  //close(fd);
  uint32_t binarySize=0;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"RResFile tryToOpenFile(\""<<name<<"\")...\n";
#endif
  fd=open(name,O_RDONLY|O_BINARY|O_RSRC);
  if(fd!=-1) {
    binarySize=lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"tryToOpenFile() File size is "<<binarySize<<"\n";
#endif
    //close(fd);
  }
  else {
    ERR_OUT<<"(RResFile::tryToOpenFile(\""<<name<<"\") failed.\n";
  }
  return binarySize;
}


// NOTE: sign is tricky...
int32_t RResFile::rdLong(long offset)
{
  uint16_t s1,s2;
  s1=rdShort(offset);
  s2=rdShort(offset+2);
  return (int32_t)(s1*65536)+s2;
}


// NOTE: sign is tricky...
int16_t RResFile::rdShort(long offset)
{
  uint8_t c1,c2;
  c1=rdChar(offset);
  c2=rdChar(offset+1);
  return (int16_t)((c1*256)+c2);
}


// NOTE: sign is tricky...
int8_t RResFile::rdChar(long offset)
{
  if(!fd) return 0;
  int8_t c=0;
  // TODO: check for errors
  lseek(fd,offset,SEEK_SET);
  read(fd,&c,1);
  return c;
}


uint32_t RResFile::rdULong(long offset)
{
  uint16_t s1,s2;
  s1=rdUShort(offset);
  s2=rdUShort(offset+2);
  return (uint32_t)(s1*65536)+s2;
}


uint16_t RResFile::rdUShort(long offset)
{
  uint8_t c1,c2;
  c1=rdUChar(offset);
  c2=rdUChar(offset+1);
  return (uint16_t)((c1*256)+c2);
}


uint8_t RResFile::rdUChar(long offset)
{
  if(!fd) return 0;
  uint8_t c=0;
  // TODO: check for errors
  lseek(fd,offset,SEEK_SET);
  read(fd,&c,1);
  return c;
}


unsigned int RResFile::countType(const char *type)
{
  if(!type) return 0;
  if(!numTypes) return 0;
  if(err.getError()) return 0;
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return 0; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return 0; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return 0; }
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"Looking for type "<<type<<" resources.\n";
#endif
  offset=resHeader->resMap->typeOffset+resHeader->mapOffset+2;
  unsigned int num=numTypes;
  while(num) {
    readResEntry();
    if(!strcmp(ttype,type)) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"Found "<<(int)resEnt->num<<" type "<<type<<" resources.\n";
#endif
      return resHeader->resEnt->num;
    }
    num--;
  }
  return 0;
}


void RResFile::printName(long off)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return; }
  offset=resHeader->mapOffset+resHeader->resMap->nameOffset+offset;
  unsigned int c=rdUChar(offset);  offset++;
  DEBUG_OUT<<"\"";
  for(unsigned int t=0;t<c;t++) {
    char cc=rdUChar(offset);  offset++;
    DEBUG_OUT<<(char)cc;
  }
  DEBUG_OUT<<"\"";
}


void RResFile::printName(char *buf)
{
  if(!buf) return;
  unsigned int c=buf[0];
  DEBUG_OUT<<"\"";
  for(unsigned int t=0;t<c;t++) {
    char cc=buf[t+1];
    DEBUG_OUT<<(char)cc;
  }
  DEBUG_OUT<<"\"";
}


bool RResFile::getRefFromNum(const char *type,unsigned int num)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!type) return false;
  bool result=true;
  unsigned int n=countType(type);
  if(!n) {
    ERR_OUT<<"No resources of type \""<<type<<"\".\n";
    return false;
  }
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"n is "<<n<<" num is "<<num<<"\n";
#endif
  if(num<=n) {
    n--;
    offset=resHeader->resMap->typeOffset+resHeader->mapOffset+resHeader->resEnt->refOffset;
    offset+=num*12;  // skip to the one we want...
    resHeader->resRef->id=rdUShort(offset);  offset+=2;
    resHeader->resRef->nameOffset=rdShort(offset);  offset+=2;
    uint32_t temp=rdLong(offset);  offset+=4;
    resHeader->resRef->attrib=(char)((temp&0xff000000)>>24);
    resHeader->resRef->dataOffset=temp&0xffffff;
    if(resHeader->resRef->dataOffset>binarySize) {
      ERR_OUT<<"getRefFromNum() dataOffset is past EOF, giving up!\n";
      exit(EXIT_FAILURE); return 0;
    }
  }
  else {
    ERR_OUT<<"No such resource with that type and number.\n";
    result=false;
  }
  if(result) {
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"getRefFromNum() resRef type=\""<<type<<"\", num="<<num<<", "<<"resHeader->resRef->id="<<(int)resHeader->resRef->id<<", ";
    //DEBUG_OUT<<"nameOffset is "<<(int)resRef->nameOffset<<" ";
#endif
    if(resHeader->resRef->nameOffset<0) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"getRefFromNum() resHeader->resRef->nameOffset=0";
#endif
   }
    else {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"getRefFromNum() resHeader->resRef->nameOffset=";
      //printName(resHeader->resRef->nameOffset);
#endif
    }
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"dataOffset is "<<(int)resRef->dataOffset<<"\n";
    //DEBUG_OUT<<"\n";
#endif
  }
  return result;
}


bool RResFile::getRefFromID(const char *type,unsigned int id)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!type) return false;
  bool result=false;
  unsigned int n=countType(type);
  if(!n) {
    ERR_OUT<<"No resources of type \""<<type<<"\".\n";
    return false;
  }
  bool looking=true;
  unsigned int counter=0;
  unsigned int tid,tnameOffset,temp;
  while(looking) {
    offset=resHeader->resMap->typeOffset+resHeader->mapOffset+resHeader->resEnt->refOffset;
    offset+=counter*12;  // skip to the next one...
    tid=rdUShort(offset);  offset+=2;
    tnameOffset=rdShort(offset);  offset+=2;
    temp=rdLong(offset);  offset+=4;
#ifdef DEBUG_VERBOSE
    //DEBUG_OUT<<"Looking at type \""<<type<<"\" id "<<tid<<"\n";
#endif
    if(tid==id) {
#ifdef DEBUG_VERBOSE
      //DEBUG_OUT<<"Found the one we were looking for!\n";
#endif
      resHeader->resRef->id=tid;
      resHeader->resRef->nameOffset=tnameOffset;
      resHeader->resRef->attrib=(char)((temp&0xff000000)>>24);
      resHeader->resRef->dataOffset=temp&0xffffff;
      if(resHeader->resRef->dataOffset>binarySize) {
        ERR_OUT<<"getRefFromID() dataOffset is past EOF, giving up!\n";
        exit(EXIT_FAILURE); return 0;
      }
      result=true;
    }
    counter++;
    if(counter==n) looking=false;
  }
  if(result) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Found resRef for type \""<<type<<"\" id "<<id<<".\n";
    DEBUG_OUT<<"getRefFromID() resHeader->resRef->nameOffset="<<(int)resHeader->resRef->nameOffset<<" ";
    if(resHeader->resRef->nameOffset<0) {
      DEBUG_OUT<<"(no name)\n";
    }
    else printName(resHeader->resRef->nameOffset);
    DEBUG_OUT<<"getRefFromID() resHeader->resRef->dataOffset="<<(int)resHeader->resRef->dataOffset<<"\n";
#endif
  }
  else {
    ERR_OUT<<"Didn't find resRef for \""<<type<<"\" id "<<id<<".\n";
  }
  return result;
}


long RResFile::getDataOffset()
{
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"getDataOffset()...\n";
#endif
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; return 0; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; return 0; }
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"getDataOffset() resHeader->dataOffset="<<resHeader->dataOffset<<"\n";
  //DEBUG_OUT<<"getDataOffset() resHeader->resRef->dataOffset="<<resHeader->resRef->dataOffset<<"\n";
#endif
  if(!resHeader->dataOffset) { ERR_OUT<<"getDataOffset() resHeader->dataOffset is zero, giving up!\n"; return 0; }
  if(!resHeader->resRef->dataOffset) { ERR_OUT<<"getDataOffset() resHeader->resRef->dataOffset is zero, giving up!\n"; return 0; }
  offset=resHeader->dataOffset+resHeader->resRef->dataOffset+4;  // 4 is for size
  if(offset>binarySize) {
    ERR_OUT<<"getDataOffset() offset is past EOF, giving up!\n";
    return 0;
  }
  return offset;
}


unsigned int RResFile::getDataSize()
{
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"getDataSize()...\n";
#endif
  getDataOffset();
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"Going to rdULong("<<offset<<"-4)...\n";
#endif
  return rdULong(offset-4);
}


bool RResFile::readFromNum(const char *type,unsigned int num, char *buffer)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!type) return false;
  if(!numTypes) return false;
  if(!buffer) return false;
  bool result=false;
  unsigned int size=0;
  if(!fd) return 0;
  result=getRefFromNum(type,num);
  if(result) {
    offset=resHeader->dataOffset+resHeader->resRef->dataOffset;
    if(offset>binarySize) {
      ERR_OUT<<"readFromNum() offset is past EOF, giving up!\n";
      exit(EXIT_FAILURE);
      return false;
    }
    size=rdULong(offset);  offset+=4;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Found resource type \""<<type<<"\" number "<<num<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
#endif
    // TODO: check for errors
    lseek(fd,offset,SEEK_SET);
    read(fd,buffer,size);
    curSize=size;
  }
  return result;
}


bool RResFile::readFromID(const char *type,unsigned int id, char *buffer)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!type) return false;
  if(!numTypes) return false;
  if(!buffer) return false;
  bool result=false;
  unsigned int size=0;
  if(!fd) return 0;
  result=getRefFromID(type,id);
  if(result) {
    offset=resHeader->dataOffset+resHeader->resRef->dataOffset;
    if(offset>binarySize) {
      ERR_OUT<<"readFromID() offset is past EOF, giving up!\n";
      exit(EXIT_FAILURE);
      return false;
    }
    size=rdULong(offset);  offset+=4;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Found resource type \""<<type<<"\" id "<<id<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
#endif
    // TODO: check for errors
    lseek(fd,offset,SEEK_SET);
    read(fd,buffer,size);
    curSize=size;
  }
  return result;
}


bool RResFile::getNameFromNum(const char *type,unsigned int num, char *buf)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!type) return false;
  if(!buf) return false;
  bool res=getRefFromNum(type,num);
  if(res) {
    offset=resHeader->mapOffset+resHeader->resMap->nameOffset+resHeader->resRef->nameOffset;
    unsigned int c=rdUChar(offset);  offset++;
    unsigned int t;
    for(t=0;t<c;t++) {
      unsigned char cc=rdUChar(offset);  offset++;
      buf[t]=cc;
    }
    buf[t]=0;
  }
  return res;
}


bool RResFile::getNameFromID(const char *type,unsigned int id, char *buf)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return false; }
  if(!type) return false;
  if(!buf) return false;
  bool res=getRefFromID(type,id);
  if(res) {
    offset=resHeader->mapOffset+resHeader->resMap->nameOffset+resHeader->resRef->nameOffset;
    unsigned int c=rdUChar(offset);  offset++;
    unsigned int t;
    for(t=0;t<c;t++) {
      unsigned char cc=rdUChar(offset);  offset++;
      buf[t]=cc;
    }
    buf[t]=0;
  }
  return res;
}


unsigned int RResFile::getSizeFromNum(const char *type,unsigned int num)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return 0; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return 0; }
  if(!type) return 0;
  unsigned int size=0;
  bool result=false;
  result=getRefFromNum(type,num);
  if(result) {
    offset=resHeader->dataOffset+resHeader->resRef->dataOffset;
    if(offset>binarySize) {
      ERR_OUT<<"getSizeFromNum() offset is past EOF, giving up!\n";
      exit(EXIT_FAILURE);
      return 0;
    }
    size=rdULong(offset);  offset+=4;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Found resource type \""<<type<<"\" number "<<num<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
#endif
    curSize=size;
    return size;
  }
  return size;
}


unsigned int RResFile::getSizeFromID(const char *type,unsigned int id)
{
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE); return 0; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE); return 0; }
  if(!type) return 0;
  unsigned int size=0;
  bool result=false;
  result=getRefFromID(type,id);
  if(result) {
    offset=resHeader->dataOffset+resHeader->resRef->dataOffset;
    if(offset>binarySize) {
      ERR_OUT<<"getSizeFromID() offset is past EOF, giving up!\n";
      exit(EXIT_FAILURE);
      return 0;
    }
    size=rdULong(offset);  offset+=4;
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"Found resource type \""<<type<<"\" id "<<id<<" it is ";
    DEBUG_OUT<<size<<" bytes long.\n";
#endif
    curSize=size;
    return size;
  }
  return size;
}


bool RResFile::readResEntry()
{
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE); return false; }
  resHeader->resEnt->type=rdULong(offset);  offset+=4;
  resHeader->resEnt->num=rdUShort(offset)+1;  offset+=2;
  resHeader->resEnt->refOffset=rdShort(offset);  offset+=2;
  ttype[4]=0;
  ttype[0]=(char)((resHeader->resEnt->type)>>24)&0xff;
  ttype[1]=(char)((resHeader->resEnt->type)>>16)&0xff;
  ttype[2]=(char)((resHeader->resEnt->type)>>8)&0xff;
  ttype[3]=(char)(resHeader->resEnt->type)&0xff;
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"type="<<ttype<<" ("<<(int)resEnt->type<<") num="<<(int)resEnt->num;
  //DEBUG_OUT<<" refOff="<<(int)resEnt->refOffset<<"\n";
#endif
  return true;
}


bool RResFile::readMacEntry()
{
  return readResEntry();
}


bool RResFile::readPalmEntry()
{
  return readResEntry();
}


const char *RResFile::getCurType()
{
  return ttype;
}


unsigned int RResFile::getCurCount()
{
  return resHeader->resEnt->num;
}


bool RResFile::readResHeader()
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"RResFile::readResHeader()...\n";
#endif
  if(!resHeader) { ERR_OUT<<"No resHeader!!!\n"; exit(EXIT_FAILURE);return false; }
  if(!resHeader->resMap) { ERR_OUT<<"No resMap!!!\n"; exit(EXIT_FAILURE);return false; }
  if(!resHeader->resEnt) { ERR_OUT<<"No resEnt!!!\n"; exit(EXIT_FAILURE);return false; }
  if(!resHeader->resRef) { ERR_OUT<<"No resRef!!!\n"; exit(EXIT_FAILURE);return false; }
  if(headerRead) {
    ERR_OUT<<"Resource Header already read!\n";
    return false;
  }
  // Read resource resHeader
  offset=0;
  resHeader->dataOffset=rdULong(offset);  offset+=4;
  resHeader->mapOffset=rdULong(offset);  offset+=4;
  resHeader->dataLen=rdULong(offset);  offset+=4;
  resHeader->mapLen=rdULong(offset);  offset+=4;
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"readResHeader() resHeader->dataOffset="<<resHeader->dataOffset<<" resHeader->dataLen="<<resHeader->dataLen<<"\n";
  //DEBUG_OUT<<"readResHeader() resHeader->mapOffset="<<resHeader->mapOffset<<" resHeader->mapLen="<<resHeader->mapLen<<"\n";
#endif
  //
  // TODO: Move this to checkSanity prob
  if(resHeader->dataOffset>binarySize) { ERR_OUT<<"resHeader->dataOffset invalid!\n"; return false; }
  if(resHeader->mapOffset>binarySize) { ERR_OUT<<"resHeader->mapOffset invalid!\n"; return false; }
  if(!resHeader->dataOffset) { ERR_OUT<<"resHeader->dataOffset zero!\n"; return false; }
  if(!resHeader->mapOffset) { ERR_OUT<<"resHeader->mapOffset zero!\n";return false; }
  //
  if(resHeader->dataLen>binarySize) { ERR_OUT<<"resHeader->dataLen invalid!\n"; return false; }
  if(resHeader->mapLen>binarySize) { ERR_OUT<<"resHeader->mapLen invalid!\n"; return false; }
  if(!resHeader->mapLen) { ERR_OUT<<"resHeader->mapLen zero!\n"; return false; }
  //
  // Read resource map
  offset=resHeader->mapOffset;
  // Skip, skip, skip...
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdLong(offset); offset+=4;
  rdShort(offset); offset+=2;
  resHeader->resMap->fileAttrib=rdUShort(offset);  offset+=2;
  resHeader->resMap->typeOffset=rdUShort(offset);  offset+=2;
  resHeader->resMap->nameOffset=rdUShort(offset);  offset+=2;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"readResHeader() resHeader->resMap->fileAttrib="<<(int)resHeader->resMap->fileAttrib<<"\n";
  DEBUG_OUT<<"readResHeader() resHeader->resMap->typeOffset="<<(int)resHeader->resMap->typeOffset<<"\n";
  DEBUG_OUT<<"readResHeader() resHeader->resMap->nameOffset="<<(int)resHeader->resMap->nameOffset<<"\n";
#endif
  //
  // TODO: Move this to checkSanity prob
  if(!resHeader->resMap->typeOffset) { ERR_OUT<<"resHeader->resMap->typeOffset zero!\n"; return false; }
  if(!resHeader->resMap->nameOffset) { ERR_OUT<<"resHeader->resMap->nameOffset zero!\n"; return false; }
  if(resHeader->resMap->typeOffset>binarySize) { ERR_OUT<<"resHeader->resMap->typeOffset invalid!\n"; return false; }
  if(resHeader->resMap->nameOffset>binarySize) { ERR_OUT<<"resHeader->resMap->nameOffset invalid!\n"; return false; }
  //
  // Read in list of resource types...
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Building list of resource types...\n";
#endif
  typeMap->jumpToHead();
  offset=resHeader->resMap->typeOffset+resHeader->mapOffset;
  numTypes=rdUShort(offset)+1; offset+=2;
  if((!resHeader->dataLen)||(!resHeader->mapLen)) numTypes=0;
  unsigned int num=numTypes;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<num<<" types of resources in map.\n";
#endif
  RResource *r=(RResource *)NULL;
  RResource *t=(RResource *)NULL;
  while(num) {
    //DEBUG_OUT<<"Going to readresEntry()...\n";
    readResEntry();
    char *theType=(char *)malloc(5); // 4 letters and null terminator
    strcpy(theType,ttype);
    t=new RResource((char *)NULL,theType,resHeader->resEnt->num,resHeader->resEnt->refOffset,0);
    typeMap->insert(t);
    num--;
  }
  // Now read in actual resource entries...
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Building resource list...\n";
#endif
  typeMap->jumpToHead();
  resourceMap->jumpToHead();
  bool reading=true;
  while(reading) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"reading typeMap...\n";
#endif
    if(typeMap->atEnd()) reading=false;
    else {
      t=(RResource *)typeMap->info();
      char *theType=strdup(t->type);
      for(unsigned int n=0;n<t->num;n++) {
#ifdef DEBUG_VERBOSE
        //DEBUG_OUT<<"Going to getRefFromNum(\""<<theType<<"\","<<n<<")...\n";
#endif
        getRefFromNum(theType,n);
#ifdef DEBUG_VERBOSE
        //DEBUG_OUT<<"Back from getRefFromNum().\n";
#endif
        const char *theName="n/a";  // TODO: not handled yet...
        unsigned int theID=resHeader->resRef->id;
        uint32_t theOff=resHeader->dataOffset+resHeader->resRef->dataOffset+4;
        if(theOff>binarySize) { ERR_OUT<<"readResHeader: theOff is past EOF, giving up!\n"; return 0; }
        unsigned int theSize=getDataSize();
#ifdef DEBUG_VERBOSE
        DEBUG_OUT<<"readResHeader() resource: \""<<theName<<"\",\""<<theType<<"\",#"<<theID<<",@"<<theOff<<","<<theSize<<" bytes\n";
#endif
        r=new RResource(theName,theType,theID,theOff,theSize);
        resourceMap->insert(r);
      }
      typeMap->advance();
    }
  }
  typeMap->jumpToHead();
  resourceMap->jumpToHead();
  headerRead=true;
  return true;
}



bool RResFile::readMacHeader()
{
  return readResHeader();
}


bool RResFile::readPalmHeader()
{
  return readResHeader();
}


bool RResFile::checkSanity()
{
  debugDump();
  return true;
}


void RResFile::debugDump()
{
  resourceMap->jumpToHead();
  bool reading=true;
  while(reading) {
    RResource *t=(RResource *)resourceMap->info();
    if(t) {
      t->debugDump();
      resourceMap->advance();
    }
    else reading=false;
    if(resourceMap->atEnd()) reading=false;
  }
}

