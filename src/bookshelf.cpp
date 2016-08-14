#include "bookshelf.h"

Bookshelf::Bookshelf()
{
  pPinIo = new PinIO();
  pPinIo->startup();

  pSocketServer = new SocketServer();
  pSocketServer->startup();

  for(int r=0; r<ROWS; r++){
    for(int c=0; c<COLS; c++){
      colours[r][c].r = 0;
      colours[r][c].g = 0;
      colours[r][c].b = 0;
    }
  }

  pPinIo->setPinMode(LIGHTPIN_CLOCK,OUTPUT);
  pPinIo->setPinMode(LIGHTPIN_DATA,OUTPUT);
  // pinMode (LIGHTPIN_DATA,OUTPUT);

  pPinIo->setPullUpDnControl(LIGHTPIN_CLOCK,1);
  pPinIo->setPullUpDnControl(LIGHTPIN_DATA,1);

  mode = "controlled";

  flushLights();

}

Bookshelf::~Bookshelf()
{
  delete(pPinIo);
  delete(pSocketServer);
}

void Bookshelf::test(){
  cout << "===== TESTING =====" << endl;
  /*ifstream file("game_data/buttons.json", ios::in|ios::binary|ios::ate);

  streampos size;
  size = file.tellg();
  char *buffer;
  buffer = new char[size];


  file.seekg (0, ios::beg);
  file.read (buffer, size);
  file.close();


  Document *document = new Document();
  document->Parse(buffer);

  if(document->IsObject()){
    cout << "Is object!" << endl;
    const Value& a =  document->FindMember("buttons")->value;
    for (SizeType i = 0; i < a.Size(); i++){
      //printf("a[%d] = %d\n", i, a[i].GetInt());
      cout << a[i]["name"].GetString() << endl;
    }
  }
  delete[] buffer;

  //memcpy(buffer, conn->content, (int) conn->content_len);
  //buffer[(int) conn->content_len] = 0;

  //Document *document = new Document();
  //document->Parse(buffer);
  //*/
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
    // if(lightsDirty){
    // }

  }
}


void Bookshelf::processWebMessages()
{
  Document* document = pSocketServer->getNextIncomingMessage();
  if(0 != document){
    // cout << "Got document " << document;

    string message = document->FindMember("message")->value.GetString();
    // cout << "Got message " << document;

    if(message.compare("ready") == 0){
      // pGameController->sendWebMessage(pGameController->buttonController()->getInfoString());
      for(int r=0; r<ROWS; r++){
        for(int c=0; c<COLS; c++){
          colours[r][c].r = 0;
          colours[r][c].g = 0;
          colours[r][c].b = 0;
        }
      }
      flushLights();

    }else if(message.compare("set_cell") == 0){
      int row = document->FindMember("row")->value.GetInt();
      int col = document->FindMember("col")->value.GetInt();

      colours[row][col].r = (char)document->FindMember("r")->value.GetInt();
      colours[row][col].g = (char)document->FindMember("g")->value.GetInt();
      colours[row][col].b = (char)document->FindMember("b")->value.GetInt();

      mode = "controlled";
      flushLights();
    }else if(message.compare("set_mode") == 0){
      mode = document->FindMember("name")->value.GetString();

      // flushLights();
    }

    //I don't like this here.
    delete(document);
  }
}

void Bookshelf::flushLights()
{
  // printf("%d",colours[0][0].r);

  for(int r=0; r<ROWS; r++){
    for(int c=0; c<COLS; c++){
      
      for(int s=0; s<LIGHTS_PER_SHELF; s++){
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
          if(bit & colours[r][c].r){
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
}
