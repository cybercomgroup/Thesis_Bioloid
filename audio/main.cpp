#include "audio.h"

using namespace std;


int main(int argc, char *argv[])
{
  FILE *fh;
  fh = fopen("goforward.raw", "rb");
  if (fh == NULL) {
      fprintf(stderr, "Unable to open input file goforward.raw\n");
      return -1;
  }
  audio a;
  a.init();
  //a.interpetFile(fh);
  a.listen();
  return 0;
}
