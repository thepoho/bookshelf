#include "bookshelf.h"

Bookshelf::Bookshelf()
{
  pPinIo = new PinIO();
  pPinIo->startup();

  pSocketServer = new SocketServer();
  pSocketServer->startup();
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
    unsigned int delta = millis - lastTickTime;
    lastTickTime = millis;

    processWebMessages();

  }
}


void Bookshelf::processWebMessages()
{
  Document* document = pSocketServer->getNextIncomingMessage();
  if(0 != document){
    // cout << "Got document " << document;

    string message = document->FindMember("message")->value.GetString();
    // cout << "Got message " << document;

    // if(message.compare("get_buttons") == 0){
    //   pGameController->sendWebMessage(pGameController->buttonController()->getInfoString());

    // }else if(message.compare("get_lamps") == 0){
    //   pGameController->sendWebMessage(pGameController->lampController()->getInfoString());

    // }else if(message.compare("get_coils") == 0){
    //   pGameController->sendWebMessage(pGameController->coilController()->getInfoString());

    // }else if(message.compare("set_lamp_state") == 0){
    //   string name = document->FindMember("name")->value.GetString();
    //   LampState state = (LampState)document->FindMember("value")->value.GetInt();
    //   pGameController->lampController()->setLampState(name, state);

    // }else if(message.compare("set_coil_state") == 0){
    //   string name = document->FindMember("name")->value.GetString();
    //   int state = document->FindMember("value")->value.GetInt();
    //   pGameController->coilController()->setCoilState(name, state);

    // }else if(message.compare("set_button_state") == 0){
    //   string name = document->FindMember("name")->value.GetString();
    //   int state = document->FindMember("value")->value.GetInt();
    //   pGameController->buttonController()->overrideButtonState(name, state);

    // }else if(message.compare("get_game_states") == 0){ //note the plural S

    //   pGameState->sendAllStatesToWeb();
    // }else if(message.compare("get_game_state") == 0){
    //   pGameState->sendToWeb();

    // }else if(message.compare("set_game_state") == 0){
    //   string state = document->FindMember("value")->value.GetString();
    //   setGameState(state);
    // }

    //I don't like this here.
    delete(document);
  }
}
