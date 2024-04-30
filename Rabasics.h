#ifndef RSRC_ABASICS_RSRC_H
#define RSRC_ABASICS_RSRC_H


// NOTE: This version of the file is not dependant on any other libs such as portable/nickcpp


// hodgepodge of useful string utilities


// Maybe I shouldn't depend on these here...
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define ATEMPSTRINGBUF_SIZE 256


////////////////////////////////////////////////////////////////////////////////
//  AStringHelper_rsrc Class
////////////////////////////////////////////////////////////////////////////////


#define DSHELPER_rsrc aDefaultStringHelper_rsrc

#define RSRC2HEX(a) DSHELPER_rsrc.myHex(a)
#define RSRC2HEX2(a) DSHELPER_rsrc.myHex2(a)
#define RSRC2HEX2NX(a) DSHELPER_rsrc.myHex2nox(a)
#define RSRC2SHEX2(a) DSHELPER_rsrc.mySHex2(a)
#define RSRC2HEX4(a) DSHELPER_rsrc.myHex4(a)
#define RSRC2SHEX4(a) DSHELPER_rsrc.mySHex4(a)
#define RSRC2HEX8(a) DSHELPER_rsrc.myHex8(a)
#define RSRC2SHEX8(a) DSHELPER_rsrc.mySHex8(a)
#define RSRC2HEX16(a) DSHELPER_rsrc.myHex16(a)
#define RSRC2SHEX16(a) DSHELPER_rsrc.mySHex16(a)


class AStringHelper_rsrc
{
public:
  AStringHelper_rsrc();
  virtual ~AStringHelper_rsrc();
  //
  void fatal(const char *msg);
  //
  unsigned int getArgc() const { return theArgc; }
  char *getArgv(unsigned int i) const { return theArgv[i]; }
  char **getAllArgv() const { return (char **)theArgv; }
  //
  bool extensionMatch(const char *filename, const char *extension);
  unsigned int countSpaces(const char *s);
  //
  // Quick convert to hex string
  char *myHex2(unsigned char val);
  char *mySHex2(unsigned char val);
  char *myHex2nox(unsigned char val);  // No prefix
  char *myHex4(unsigned short val);
  char *mySHex4(unsigned short val);
  char *myHex8(unsigned long val);
  char *mySHex8(unsigned long val);
  char *myHex16(unsigned long val);
  char *mySHex16(unsigned long val);
  char *myHexAddr(unsigned long val); // will vary in size
  char *myHexData(unsigned long val); // will vary in size
  char *myHex(unsigned long val);  // No leading zeros
  char *myDec(unsigned long val);
  //
  void setNumAddrDigits(unsigned int b) { numAddrDigits=b; }
  void setNumDataDigits(unsigned int b) { numDataDigits=b; }
  //
  // These all don't modify the source, but instead return a new modified string
  char *removeQuotes(const char *s);
  char *removeSpaces(const char *s);
  char *removeSpaces2(const char *s);
  char *flipSlashes(const char *s);
  char *toLower(const char *us);
  char *toUpper(const char *us);
  //
    //
  bool getDecMode() { return decMode; }
  void setDecMode(bool tMode=true) { decMode=tMode; }
  void setHexMode() { decMode=false; }
  //
  // Break up a string at whitespace...args are in theArgc and theArgv
  void parseString(const char *str, const char *cmd, bool ignoreSpaces=false, char treatThisAsASpace=' ');
  unsigned int parseNumber(const char *str);
  //
  // NOTE: These return false if they didn't find anything to do.
  static bool getPathFromFullName(char *dest,const char *fullname);
  static bool fillInPathFromFullName(char *dest,const char *fullname);
  static void fillInString(char *dest,const char *str);
  static bool getNameFromFullName(char *dest,const char *fullname);
  static bool fillInNameFromFullName(char *dest, const char *fullname);
  //
protected:
  // Private data members
  bool decMode;
  unsigned int theArgc;
  char *theArgv[16];
  char tempStringBuf[ATEMPSTRINGBUF_SIZE];
  char tempStringBuf2[ATEMPSTRINGBUF_SIZE];
  const char *hexPrefix;
  unsigned int numAddrDigits;
  unsigned int numDataDigits;
};


extern AStringHelper_rsrc DSHELPER_rsrc;


////////////////////////////////////////////////////////////////////////////////
//  AError_rsrc Class
////////////////////////////////////////////////////////////////////////////////


// NOTE: You can't reset the flag, on purpose, because all errors are considered fatal.

class AError_rsrc
{
public:
  AError_rsrc();
  bool getError();
  void setError();
protected:
  bool errorFlag;
};


////////////////////////////////////////////////////////////////////////////////
//  ADataSourceAbstract_rsrc Class
////////////////////////////////////////////////////////////////////////////////


class ADataSourceAbstract_rsrc
{
public:
  ADataSourceAbstract_rsrc();
  ADataSourceAbstract_rsrc(char *name,char *table,char *host=NULL,char *user=NULL,char *password=NULL);
  virtual ~ADataSourceAbstract_rsrc();
  virtual void kill();
  virtual bool sort();
  virtual void jumpToHead();
  virtual void jumpToTail();
  virtual void insert(void *n);
  virtual void append(void *n);
  virtual void *remove();
  virtual void *info();
  virtual void advance();
  virtual void retreat();
  virtual bool placeBookmark();
  virtual bool gotoBookmark();
  virtual bool atEnd();
  virtual bool isEmpty();
  virtual bool isSorted();
  virtual bool isConnected();
  //
  AError_rsrc err;
  //
protected:
  virtual void init();
  //
  bool connected;
  char *name;
  char *table;
  char *host;
  char *user;
};


////////////////////////////////////////////////////////////////////////////////
//  AGenericListNode_rsrc Class
////////////////////////////////////////////////////////////////////////////////


// values returned by comparision functions
#define A_COMPARE_UNKNOWN  42
#define A_COMPARE_SAME     0
#define A_COMPARE_BEFORE   -1
#define A_COMPARE_AFTER    1


// The idea is that this is what *info points to...in
// practice this class is never used...
class AGenericListNode_rsrc
{
public:
  AGenericListNode_rsrc();
  virtual ~AGenericListNode_rsrc();
  virtual int compare(AGenericListNode_rsrc *other);
protected:
};


////////////////////////////////////////////////////////////////////////////////
//  AListNode_rsrc Class
////////////////////////////////////////////////////////////////////////////////


// This is used internally by AList_rsrc to store nodes...
class AListNode_rsrc
{
friend class AList_rsrc;
public:
  //
  AListNode_rsrc();
  //
protected:
  //
  void *info;  // could be a pointer to AGenericListNode_rsrc or some such
  AListNode_rsrc *prev;
  AListNode_rsrc *next;
  //
};


////////////////////////////////////////////////////////////////////////////////
//  AList_rsrc Class
////////////////////////////////////////////////////////////////////////////////


class AList_rsrc : public ADataSourceAbstract_rsrc
{
public:
  //
  AList_rsrc();
  virtual ~AList_rsrc();
  virtual void kill();
  virtual void insert(void *n); // insert before current position
  virtual void append(void *n); // append after current position
  virtual void *remove(); // remove current position
  virtual void *info(); // info of current element
  virtual void advance(); // advance current position
  virtual void retreat(); // retreat current position
  virtual bool sort() { return false; /* list is always unsorted */ }
  virtual void jumpToHead();
  virtual void jumpToTail();
  virtual bool placeBookmark();
  virtual bool gotoBookmark();
  virtual bool atEnd(); // NOTE: true for either end of list (WHAT? NO!)
  virtual bool isEmpty() { return !head; }
  virtual bool isSorted() { return false; }
  //
protected:
  //
  virtual void init();
  //
  AListNode_rsrc *head;
  AListNode_rsrc *tail;
  AListNode_rsrc *cur; // current position
  AListNode_rsrc *bookmark;
  //
};


#endif


