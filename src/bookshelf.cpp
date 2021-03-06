#include "bookshelf.h"

Bookshelf::Bookshelf(){

}

Bookshelf::~Bookshelf(){
  delete(pPinIo);
  delete(pSocketServer);
}

void Bookshelf::startup(){
  setupPinIO();
  setupSocketServer();
  initializeLights();

  mode = "controlled";

  lightsDirty = false;

  flushLights();
}

void Bookshelf::setupPinIO(){
  pPinIo = new PinIO();
  pPinIo->startup();
  pPinIo->setPinMode(LIGHTPIN_CLOCK,OUTPUT);
  pPinIo->setPinMode(LIGHTPIN_DATA,OUTPUT);

  pPinIo->setPullUpDnControl(LIGHTPIN_CLOCK,0);
  pPinIo->setPullUpDnControl(LIGHTPIN_DATA, 0);
}

void Bookshelf::setupSocketServer(){
  pSocketServer = new SocketServer();
  pSocketServer->startup();
}
void Bookshelf::initializeLights(){
  for(int r=0; r<ROWS; r++){
    for(int c=0; c<COLS; c++){
      colours[r][c].r = 0;
      colours[r][c].g = 0;
      colours[r][c].b = 0;
    }
  }
}

void Bookshelf::test(){
  setupPinIO();

  cout << "===== TESTING =====" << endl;

  int lights = 10;
  for(int i=0; i< lights; i++){
    for(int j=0; j<8; j++){
      pPinIo->pinWrite(LIGHTPIN_DATA, 0);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 0);
    }
    for(int j=0; j<8; j++){
      pPinIo->pinWrite(LIGHTPIN_DATA, 0);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 0);
    }
    for(int j=0; j<8; j++){
      pPinIo->pinWrite(LIGHTPIN_DATA, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 0);
    }
  }

  lights = 32;
  int bits = 8;
  for(int i=0; i< lights; i++){
    for(int j=0; j<bits; j++){
      pPinIo->pinWrite(LIGHTPIN_DATA, 0);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 0);
    }
    for(int j=0; j<bits; j++){
      pPinIo->pinWrite(LIGHTPIN_DATA, 0);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 0);
    }
    for(int j=0; j<bits; j++){
      pPinIo->pinWrite(LIGHTPIN_DATA, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 1);
      pPinIo->pinWrite(LIGHTPIN_CLOCK, 0);
    }
  }

  cout << "===== TESTING COMPLETE =====" << endl;
  lightsDirty = false;
  exit(1);

}

void Bookshelf::run(){
  printf("starting main loop\n");


  //pGameController->delayedEventController()->createEvent("fire_coil", (100), "right_flipper");
  while(1){

    //work out deltas
    unsigned int millis = pPinIo->getMillis();
    // unsigned int delta = millis - lastTickTime;
    lastTickTime = millis;

    processWebMessages();

    if(mode.compare("rainbow") == 0){
      //Do the rainbow cycle here
      flushLights();
    }
    if(lightsDirty){
      flushLights();
    }

  }
}


void Bookshelf::processWebMessages()
{
  Document* document = pSocketServer->getNextIncomingMessage();
  while(0 != document){
    // cout << "Got document " << document;

    string message = document->FindMember("message")->value.GetString();
    // cout << "Got message " << document;

    if(message.compare("ready") == 0){
      // pGameController->sendWebMessage(pGameController->buttonController()->getInfoString());
      for(int r=0; r<ROWS; r++){
        for(int c=0; c<COLS; c++){
          colours[r][c].r = (char)0;
          colours[r][c].g = (char)0;
          colours[r][c].b = (char)0;
        }
      }
      flushLights();

    }else if(message.compare("set_cell") == 0){
      int row = document->FindMember("row")->value.GetInt();
      int col = document->FindMember("col")->value.GetInt();

      colours[row][col].r = (char)document->FindMember("r")->value.GetInt();
      colours[row][col].g = (char)document->FindMember("g")->value.GetInt();
      colours[row][col].b = (char)document->FindMember("b")->value.GetInt();

      // cout << row << ":" << col << " RGB " << (int)colours[row][col].r << ":" << (int)colours[row][col].g << ":" << (int)colours[row][col].b << endl;

      mode = "controlled";
      lightsDirty = true;
      // flushLights();
    }else if(message.compare("set_mode") == 0){
      mode = document->FindMember("name")->value.GetString();

      // flushLights();
    }else if(message.compare("shutdown") == 0){
      printf("Shutdown!!");
      system("shutdown -h now");
      // flushLights();
    }

    //I don't like this here.
    delete(document);
    document = pSocketServer->getNextIncomingMessage();
  }
}

void Bookshelf::flushLights()
{
  // printf("%d",colours[0][0].r);
  cout << "Flushing lights" << endl;

  for(int r=0; r<ROWS; r++){
    for(int c=0; c<COLS; c++){

      for(int s=0; s<LIGHTS_PER_SHELF; s++){
        for(int i=0; i<8; i++){
          char bit = (1<<(7-i));
          if(bit & colours[r][c].r){
            pPinIo->pinWrite(LIGHTPIN_DATA, HIGH);
          }else{
            pPinIo->pinWrite(LIGHTPIN_DATA, LOW);
          }
          pPinIo->pinWrite(LIGHTPIN_CLOCK, HIGH);
          pPinIo->pinWrite(LIGHTPIN_CLOCK, LOW);
        }

        for(int i=0; i<8; i++){
          char bit = (1<<(7-i));
          if(bit & colours[r][c].g){
            pPinIo->pinWrite(LIGHTPIN_DATA, HIGH);
          }else{
            pPinIo->pinWrite(LIGHTPIN_DATA, LOW);
          }
          pPinIo->pinWrite(LIGHTPIN_CLOCK, HIGH);
          pPinIo->pinWrite(LIGHTPIN_CLOCK, LOW);
        }

        for(int i=0; i<8; i++){
          char bit = (1<<(7-i));
          if(bit & colours[r][c].b){
            pPinIo->pinWrite(LIGHTPIN_DATA, HIGH);
          }else{
            pPinIo->pinWrite(LIGHTPIN_DATA, LOW);
          }
          pPinIo->pinWrite(LIGHTPIN_CLOCK, HIGH);
          pPinIo->pinWrite(LIGHTPIN_CLOCK, LOW);
        }
      }
    }
  }
  pPinIo->doDelay(1);
  lightsDirty = false;
}
