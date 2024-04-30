
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


// NOTE: This version of the file is not dependant on any other libs such as portable/nickcpp


#include <stdio.h>
#include <stdlib.h>

#include "../cpp/alib/adebug.h"

#include "Rabasics.h"


////////////////////////////////////////////////////////////////////////////////
//  AError_rsrc Class
////////////////////////////////////////////////////////////////////////////////

AError_rsrc::AError_rsrc()
{
  errorFlag=false;
}


bool AError_rsrc::getError()
{
  return errorFlag;
}


void AError_rsrc::setError()
{
  errorFlag=true;
  //exit(EXIT_FAILURE);
}


////////////////////////////////////////////////////////////////////////////////
//  ADataSourceAbstract_rsrc Class
////////////////////////////////////////////////////////////////////////////////

ADataSourceAbstract_rsrc::ADataSourceAbstract_rsrc()
{
  init();
}


ADataSourceAbstract_rsrc::ADataSourceAbstract_rsrc(char *tname,char *ttable,char *thost,
  char *tuser,char *tpass)
{
  init();
  connected=false;
  name=tname;
  host=thost;
  user=tuser;
  table=ttable;
}


void ADataSourceAbstract_rsrc::init()
{
  connected=true;
  name=NULL;
  host=NULL;
  user=NULL;
  table=NULL;
}


ADataSourceAbstract_rsrc::~ADataSourceAbstract_rsrc()
{
  // Nothing to do
}


void ADataSourceAbstract_rsrc::kill()
{
  // Nothing to do
}


void ADataSourceAbstract_rsrc::insert(void *n)
{
  // Nothing to do
}


void ADataSourceAbstract_rsrc::append(void *n)
{
  // Nothing to do
}


void* ADataSourceAbstract_rsrc::remove()
{
  // Because there's nothing there...
  return NULL;
}


void* ADataSourceAbstract_rsrc::info()
{
  // Because there's nothing there...
  return NULL;
}


void ADataSourceAbstract_rsrc::advance()
{
  // Nothing to do
}


void ADataSourceAbstract_rsrc::retreat()
{
  // Nothing to do
}


void ADataSourceAbstract_rsrc::jumpToHead()
{
  // Nothing to do
}


void ADataSourceAbstract_rsrc::jumpToTail()
{
  // Nothing to do
}


bool ADataSourceAbstract_rsrc::sort()
{
  // Nothing to do
  return false;
}


bool ADataSourceAbstract_rsrc::atEnd()
{
  // Because there's nothing there...
  return true;
}


bool ADataSourceAbstract_rsrc::isEmpty()
{
  // Because there's nothing there...
  return true;
}


bool ADataSourceAbstract_rsrc::isSorted()
{
  // Because there's nothing there...
  return true;
}


bool ADataSourceAbstract_rsrc::placeBookmark()
{
  // Because there's nothing there...
  return false;
}


bool ADataSourceAbstract_rsrc::gotoBookmark()
{
  // Because there's nothing there...
  return false;
}


bool ADataSourceAbstract_rsrc::isConnected()
{
  // Because we're always "connected"...and always local...by default.
  return true;
}


////////////////////////////////////////////////////////////////////////////////
//  AGenericListNode_rsrc Class
////////////////////////////////////////////////////////////////////////////////

AGenericListNode_rsrc::AGenericListNode_rsrc()
{
  // child classes might do something here...
}


AGenericListNode_rsrc::~AGenericListNode_rsrc()
{
  // child classes might do something here...
}


int AGenericListNode_rsrc::compare(AGenericListNode_rsrc *other)
{
  return A_COMPARE_UNKNOWN;
}


////////////////////////////////////////////////////////////////////////////////
//  AListNode_rsrc Class
////////////////////////////////////////////////////////////////////////////////

AListNode_rsrc::AListNode_rsrc()
{
  prev=(AListNode_rsrc *)NULL;
  next=(AListNode_rsrc *)NULL;
  info=(void *)NULL;
}


////////////////////////////////////////////////////////////////////////////////
//  AList_rsrc Class
////////////////////////////////////////////////////////////////////////////////

AList_rsrc::AList_rsrc()
{
  init();
}


void AList_rsrc::init()
{
  ADataSourceAbstract_rsrc::init();
  head=(AListNode_rsrc *)NULL;
  tail=(AListNode_rsrc *)NULL;
  cur=(AListNode_rsrc *)NULL;
  bookmark=(AListNode_rsrc *)NULL;
}


AList_rsrc::~AList_rsrc()
{
  kill();
}


void AList_rsrc::kill()
{
  if(head) {
  AListNode_rsrc *p=head;
    while(p) {
      AListNode_rsrc *q=p;
      if(p) p=p->next;
      if(q) delete q;
    }
  }
  head=(AListNode_rsrc *)NULL;
  tail=(AListNode_rsrc *)NULL;
  cur=(AListNode_rsrc *)NULL;
}


void AList_rsrc::insert( void *n )
{
  AListNode_rsrc *p=new AListNode_rsrc;
  if(!p) return;
  p->info=n;
  p->next=cur;
  if(cur) p->prev=cur->prev;
  if(p->next) p->next->prev=p;
  if(p->prev) p->prev->next=p;
  cur=p;
  if(!p->prev) head=p;
  if(!p->next) tail=p;
}


void AList_rsrc::append(void *n)
{
  if(!cur) cur=tail;
  AListNode_rsrc *p=new AListNode_rsrc;
  if(!p) return;
  p->info=n;
  p->prev=cur;
  if(cur) p->next=cur->next;
  if(p->next) p->next->prev=p;
  if(p->prev) p->prev->next=p;
  cur=p;
  if(!p->prev) head=p;
  if(!p->next) tail=p;
}


// FIXME: I'm still a little iffsety on this one...
void *AList_rsrc::remove()
{
  if(!head) return NULL;
  if(!tail) return NULL;
  if(!cur) cur=tail;
  AListNode_rsrc *p=cur;
  void *r=cur->info;
  if(p->next) p->next->prev=p->prev;
  if(p->prev) p->prev->next=p->next;
  if(p->next) cur=p->next;
  else cur=p->prev;
  //if(!head->prev) head=p->next;
  //if(!tail->next) tail=p->prev;
  if(!p->next) tail=p->prev;
  if(!p->prev) head=p->next;
  if(!cur) head=(AListNode_rsrc *)NULL;
  if(!cur) tail=(AListNode_rsrc *)NULL;
  delete p;
  return r;
}


void *AList_rsrc::info()
{
  void *ret=NULL;
  if(cur) ret=cur->info;
  return ret;
}


void AList_rsrc::advance()
{
  if(cur) {
     cur=cur->next;
  }
}


void AList_rsrc::retreat()
{
  if(cur) {
     cur=cur->prev;
  }
}


void AList_rsrc::jumpToHead()
{
  cur=head;
}


void AList_rsrc::jumpToTail()
{
  cur=tail;
}


bool AList_rsrc::atEnd()
{
  bool ret=false;
  if(!cur) {
    ret=true;
  }
  return ret;
}


bool AList_rsrc::placeBookmark()
{
  bookmark=cur;
  return true;
}


bool AList_rsrc::gotoBookmark()
{
  cur=bookmark;
  return true;
}


AStringHelper_rsrc DSHELPER_rsrc;


////////////////////////////////////////////////////////////////////////////////
//  AStringHelper_rsrc Class
////////////////////////////////////////////////////////////////////////////////


AStringHelper_rsrc::AStringHelper_rsrc()
{
  decMode=true;
  //hexPrefix="$";
  //hexPrefix="0x";
  hexPrefix="";
  // Assume 32 bits for now...
  numAddrDigits=8;
  numDataDigits=8;
  theArgc=0;
  for(unsigned int t=0;t<16;t++) theArgv[t]=NULL;
}


AStringHelper_rsrc::~AStringHelper_rsrc()
{
  //ERR_OUT<<"AStringHelper_rsrc::~AStringHelper_rsrc()\n";
}


void AStringHelper_rsrc::fatal(const char *msg)
{
  ERR_OUT<<msg<<"\n";
  exit(EXIT_FAILURE);
}


unsigned int AStringHelper_rsrc::countSpaces(const char *s)
{
  // TODO: Count contiguous spaces as a single space...?
  unsigned int ret=0;
  unsigned long l=strlen(s);
  for(unsigned int t=0;t<l;t++)
    if(s[t]==' ') ret++;
  return ret;
}


char *AStringHelper_rsrc::removeQuotes(const char *s)
{
  if(!s) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  unsigned long len=strlen(s);
  unsigned int t=0,tt=0;
  while(t<len) {
    if(s[t]!='\"') {
      tempStringBuf[tt]=s[t];
      tt++;
    }
    t++;
  }
  return tempStringBuf;
}


char *AStringHelper_rsrc::flipSlashes(const char *s)
{
  if(!s) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  strcpy(tempStringBuf,s);
  unsigned long len=strlen(tempStringBuf);
  for(unsigned int t=0;t<len;t++) {
    char c=tempStringBuf[t];
    if(c=='\\') tempStringBuf[t]='/';
    if(c=='/') tempStringBuf[t]='\\';
  }
  return tempStringBuf;
}


bool AStringHelper_rsrc::extensionMatch(const char *filename, const char *extension)
{
  bool ret=false;
  unsigned long l=strlen(filename);
  unsigned long e=strlen(extension);
#ifdef WIN32
  if(!_strnicmp(filename+l-e,extension,e)) ret=true;
#else
  if(!strncasecmp(filename+l-e,extension,e)) ret=true;
#endif
  return ret;
}


// NOTE: This is designed to parse a single line (only one newline)
// TODO: Handle escapes
void AStringHelper_rsrc::parseString(const char *str, const char *cmd, bool ignoreSpaces,char treatThisAsASpace)
{
  // States:
  // false - base
  // true - in quote, waiting for close quote
  bool state=false;
  unsigned int t=0;
  theArgc=0;
  for(t=0;t<16;t++) theArgv[t]=NULL;
  //CONS_OUT<<"AStringHelper_rsrc::parseString()...\n";
  unsigned int n=0;
  if(cmd) { n++; theArgv[0]=(char *)cmd; theArgc=1; }
  if(!str) return;
  unsigned long arglen=strlen(str);
  if(!arglen) return;
  char *src=strdup(str);
  //CONS_OUT<<"AStringHelper_rsrc::parseString ("<<arglen<<" chars incl nl): '"<<src<<"'\n";
  for(t=0;t<arglen;t++) {
    if(src[t]=='"') {
      //CONS_OUT<<"double quote!\n";
      state=!state;
      src[t]=0;
    }
    if((src[t]==treatThisAsASpace)&&treatThisAsASpace) {
      //CONS_OUT<<"treatThisAsASpace!\n";
      src[t]=0;
    }
    if(src[t]=='\t') {
      //CONS_OUT<<"tab!\n";
      src[t]=0;
    }
    if(src[t]==10) {
      //CONS_OUT<<"10!\n";
      src[t]=0;
    }
    if(src[t]==13) {
      //CONS_OUT<<"13!\n";
      src[t]=0;
    }
    if(src[t]==' ') {
      //CONS_OUT<<"space!\n";
      if(!state) {
        //CONS_OUT<<"!state!\n";
        if(!ignoreSpaces) {
          //CONS_OUT<<"!ignoreSpaces!\n";
          src[t]=0;
        }
        //else CONS_OUT<<"just ignored a space!\n";
      }
    }
  }
  if(strlen(src)) theArgv[n]=src; else n--;
  for(t=0;t<arglen;t++) {
    if(!src[t]) {
      //CONS_OUT<<strlen(src+t+1)<<" "<<((char *)src+t+1)<<"\n";
      if(strlen(src+t+1)) { n++; theArgv[n]=src+t+1; }
    }
  }
  theArgc=n+1;
  //CONS_OUT<<"theArgc is "<<theArgc<<"\n";
  for(t=0;t<theArgc;t++) {
    if(theArgv[t]) {
      //CONS_OUT<<t<<": '"<<theArgv[t]<<"' ("<<((long)strlen(theArgv[t]))<<" chars)\n";
    }
  }
}


unsigned int AStringHelper_rsrc::parseNumber(const char *str)
{
  if(!str) return 0;
  //if(str[strlen(str)-1]=='h') str[strlen(str)-1]=0;
  unsigned int val=0,offset=0;
  char *format=NULL;
  bool tDecMode=decMode;
  if(str[0]=='#') { tDecMode=true;  offset=1; }
  if(str[0]=='$') { tDecMode=false;  offset=1; }
  if((str[0]=='0')&&(str[1]=='x')) { tDecMode=false;  offset=2; }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Parsing number '"<<str<<"' (tDecMode="<<tDecMode<<") ";
#endif
  if(tDecMode) format=(char *)"%d"; else format=(char *)"%x";
  sscanf(str+offset,format,&val);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"is "<<val<<"\n";
#endif
  return val;
}


char *AStringHelper_rsrc::toLower(const char *us)
{
  if(!us) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  strcpy(tempStringBuf,us);
  unsigned long l=strlen(tempStringBuf);
  for(unsigned int t=0;t<l;t++) {
    if((tempStringBuf[t]>='A')&&(tempStringBuf[t]<='Z'))
      tempStringBuf[t]=(char)(tempStringBuf[t]+('a'-'A'));
  }
  return tempStringBuf;
}


char *AStringHelper_rsrc::toUpper(const char *ls)
{
  if(!ls) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  strcpy(tempStringBuf,ls);
  unsigned long l=strlen(tempStringBuf);
  for(unsigned int t=0;t<l;t++) {
    if((tempStringBuf[t]>='a')&&(tempStringBuf[t]<='z'))
      tempStringBuf[t]=(char)(tempStringBuf[t]-('a'-'A'));
  }
  return tempStringBuf;
}


char *AStringHelper_rsrc::removeSpaces(const char *s)
{
  if(!s) {
    tempStringBuf[0]=0;
    return tempStringBuf;
  }
  unsigned long l=strlen(s),i=0;
  for(unsigned int t=0;t<l;t++) {
    if(s[t]!=' ') {
      tempStringBuf[i]=s[t];  i++;
    }
  }
  tempStringBuf[i]=0;
  return tempStringBuf;
}


char *AStringHelper_rsrc::removeSpaces2(const char *s)
{
  if(!s) {
    tempStringBuf2[0]=0;
    return tempStringBuf2;
  }
  unsigned long l=strlen(s),i=0;
  for(unsigned int t=0;t<l;t++) {
    if(s[t]!=' ') {
      tempStringBuf2[i]=s[t];  i++;
    }
  }
  tempStringBuf2[i]=0;
  return tempStringBuf2;
}


/* STATIC */
bool AStringHelper_rsrc::getPathFromFullName(char *dest,const char *fullname)
{
  dest[0]=0;
  return fillInPathFromFullName(dest,fullname);
}


/* STATIC */
bool AStringHelper_rsrc::fillInPathFromFullName(char *dest,const char *fullname)
{
  if(!fullname) return false;
  if(!dest) return false;
  unsigned long slen=strlen(fullname);
  unsigned int n=0,t=0;
  for(t=0;t<slen;t++) {
    //DEBUG_OUT<<"Looking at :"<<fullname[t]<<"\n";
    if(fullname[t]=='=') n=t; // hehe...
    if(fullname[t]==':') n=t;
    if(fullname[t]=='/') n=t;
    if(fullname[t]=='\\') n=t;
  }
  // Don't do anything if there isn't a leading path in the pathname...
  if(!n) return false;
  //DEBUG_OUT<<"Looks like the offset is "<<n<<"\n";
  unsigned long dlen=strlen(dest);
  for(t=0;t<n;t++) dest[dlen+t]=fullname[t];
  dest[n+dlen]=0;
  /*
  char tc=fullname[n+1];
  fullname[n+1]=0;
  char *tname=fullname;
  //DEBUG_OUT<<"tname is \""<<tname<<"\"\n";
  fillInString(dest,tname);
  fullname[n+1]=tc;
  */
  return true;
}


/* STATIC */
void AStringHelper_rsrc::fillInString(char *dest,const char *str)
{
  if(!str) return;
  if(!dest) return;
  unsigned long dlen=strlen(dest);
  unsigned long slen=strlen(str);
  //DEBUG_OUT<<"Len of src is "<<slen<<", dest is "<<dlen<<"\n";
  for(unsigned int t=0;t<slen;t++) dest[dlen+t]=str[t];
  dest[slen+dlen]=0;
}


/* STATIC */
bool AStringHelper_rsrc::getNameFromFullName(char *dest,const char *fullname)
{
  if(!fullname) return false;
  if(!dest) return false;
  dest[0]=0;
  return fillInNameFromFullName(dest,fullname);
}


/* STATIC */
bool AStringHelper_rsrc::fillInNameFromFullName(char *dest,const char *fullname)
{
  if(!fullname) return false;
  if(!dest) return false;
  unsigned long slen=strlen(fullname);
  unsigned int n=0;
  for(unsigned int t=0;t<slen;t++) {
    //DEBUG_OUT<<"Looking at :"<<fullname[t]<<"\n";
    if(fullname[t]=='=') n=t; // hehe...
    if(fullname[t]==':') n=t;
    if(fullname[t]=='/') n=t;
    if(fullname[t]=='\\') n=t;
  }
  //DEBUG_OUT<<"Looks like the offset is "<<n<<"\n";
  unsigned int p=1;
  // If there isn't a leading path in the pathname don't skip anything...
  if(!n) p=0;
  const char *tname=fullname+n+p;
  //DEBUG_OUT<<"tname is \""<<tname<<"\"\n";
  fillInString(dest,tname);
  return true;
}


char *AStringHelper_rsrc::myDec(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%ld",val);
  return tempStringBuf;
}


// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::myHex(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%lx",hexPrefix,val);
  return tempStringBuf;
}


// NOTE: number of digits (bits) will vary
// This is for converting an address to hex text
char *AStringHelper_rsrc::myHexAddr(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  switch(numAddrDigits)
  {
    case 4: sprintf(tempStringBuf,"%s%04lx",hexPrefix,val); break;
    case 8: sprintf(tempStringBuf,"%s%08lx",hexPrefix,val); break;
    case 16: sprintf(tempStringBuf,"%s%016lx",hexPrefix,val); break;
    default: sprintf(tempStringBuf,"%s%08lx",hexPrefix,val); break;
  }
  return tempStringBuf;
}


// NOTE: number of digits (bits) will vary
// This is for converting a data to hex text
char *AStringHelper_rsrc::myHexData(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  switch(numDataDigits)
  {
    case 4: sprintf(tempStringBuf,"%s%04lx",hexPrefix,val); break;
    case 8: sprintf(tempStringBuf,"%s%08lx",hexPrefix,val); break;
    case 16: sprintf(tempStringBuf,"%s%016lx",hexPrefix,val); break;
    default: sprintf(tempStringBuf,"%s%08lx",hexPrefix,val); break;
  }
  return tempStringBuf;
}


// NOTE: unsigned 64 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::myHex16(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%016lx",hexPrefix,val);
  return tempStringBuf;
}


// TODO: Sign is not actually implemented yet!!!
// NOTE: signed 64 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::mySHex16(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%016lx",hexPrefix,val);
  return tempStringBuf;
}


// NOTE: unsigned 32 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::myHex8(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%08lx",hexPrefix,val);
  return tempStringBuf;
}


// TODO: Sign is not actually implemented yet!!!
// NOTE: signed 32 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::mySHex8(unsigned long val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%08lx",hexPrefix,val);
  return tempStringBuf;
}


// NOTE: unsigned 16 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::myHex4(unsigned short val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%04x",hexPrefix,val);
  return tempStringBuf;
}


// TODO: Sign is not actually implemented yet!!!
// NOTE: signed 16 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::mySHex4(unsigned short val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%04x",hexPrefix,val);
  return tempStringBuf;
}


// NOTE: unsigned 8 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::myHex2(unsigned char val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%02x",hexPrefix,val);
  return tempStringBuf;
}


// TODO: Sign is not actually implemented yet!!!
// NOTE: signed 8 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::mySHex2(unsigned char val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%s%02x",hexPrefix,val);
  return tempStringBuf;
}


// NOTE: unsigned 8 bits
// This is for converting an arbitrary pointer to a hex string...
char *AStringHelper_rsrc::myHex2nox(unsigned char val)
{
  // NOTE: Since this is using a common buffseter, cache results
  // if you're going to use this multiple times with stdio before a flush!
  tempStringBuf[0]=0;
  sprintf(tempStringBuf,"%02x",val);
  return tempStringBuf;
}


#ifdef UNICODE
wchar_t *AStringHelper_rsrc::toWide(char *ss,unsigned int offset)
{
  if(!ss) {
    unicodew[0]=0;  unicodew[1]=0;
    return unicodew;
  }
  unsigned int len=strlen(ss);
  for(unsigned int t=0;t<len;t++) {
    unicodew[offset+t]=ss[t];
  }
  unicodew[offset+len]=0;
  return unicodew+offset;
}
#endif // UNICODE


#ifdef UNICODE
char *AStringHelper_rsrc::toNarrow(wchar_t *ws,unsigned int offset)
{
  if(!ws) {
    unicodes[0]=0;
    return unicodes;
  }
  unsigned int len=wcslen(ws);
  for(unsigned int t=0;t<len;t++) {
    unicodes[offset+t]=(char)ws[t];
  }
  unicodes[offset+len]=0;
  return unicodes+offset;
}
#endif // UNICODE


