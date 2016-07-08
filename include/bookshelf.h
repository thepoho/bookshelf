#ifndef _BOOKSHELF_H
#define _BOOKSHELF_H

#include "common_defines.h"

#include "socket_server.h"
#include "pin_io.h"


class Bookshelf
{
public:
  Bookshelf();
  ~Bookshelf();
  void run();
  void test();

  void processWebMessages();

  // PinIO* pinIO() { return pPinIo; }
  // SocketServer*             socketServer()           { return pSocketServer; }

private:
  unsigned int lastTickTime;

  PinIO* pPinIo;
  SocketServer* pSocketServer;

};

#endif //_BOOKSHELF_H
