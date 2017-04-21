#include "image.h"
int main( int argc, char** argv )
{
    bool rot = false;
    if(argc > 1){
      if(argv[1][0] == 'r'){
        rot = true;
      }
    }

    capture(640,360, rot);
}
