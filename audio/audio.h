#include <string>
#include <iostream>


class audio {
public:
  int init();
  void listen();
  int interpetFile(FILE *fh);
  void destroy();
};
