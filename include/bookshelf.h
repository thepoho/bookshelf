#ifndef _BOOKSHELF_H
#define _BOOKSHELF_H

#include "common_defines.h"


class Bookshelf
{
public:
  Bookshelf();
  ~Bookshelf();
  void run();
  void test();

  void processWebMessages();

private:
  unsigned int lastTickTime;

};

#endif //_BOOKSHELF_H
