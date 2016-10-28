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
  void startup();

  // PinIO* pinIO() { return pPinIo; }
  // SocketServer*             socketServer()           { return pSocketServer; }

private:
void setupPinIO();
void setupSocketServer();
void initializeLights();


 struct Colour {
   char r;
   char g;
   char b;
 };

  Colour colours[ROWS][COLS];

  unsigned int lastTickTime;

  PinIO* pPinIo;
  SocketServer* pSocketServer;

  void flushLights();

  string mode;

  bool lightsDirty;

};

#endif //_BOOKSHELF_H
