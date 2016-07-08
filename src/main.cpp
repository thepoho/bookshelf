#include "bookshelf.h"

int main(int argc, char* argv[]){
  Bookshelf* pBookshelf = new Bookshelf();
  if(argc == 2 && std::string(argv[1]) == "--test"){
    pBookshelf->test();
  }else{
    pBookshelf->run();
  }
}
