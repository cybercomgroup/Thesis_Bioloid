#include "rs232.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      1      // or whatever size of buffer you want to send

void testMode();

//Parser and global belonging variables
void cParser(int argN, char *argv[]);

bool test = false;
int comport = 16;
int baudrate = 57600;

/*
Usage:

*/
int main (int argc, char *argv[]) {
  //Set up:
  if(argc > 1)
  {
    cParser((argc),argv);
  }

  cout<<"The program is running these values:"<<endl;
  cout<<"Testmode: "<<test<<endl;
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;


  unsigned char receive_buffer[RECEIVE_CHARS];
  unsigned char send_byte = 42;
  unsigned char send_buffer[SEND_CHARS] = {'l'};
  unsigned char send_buffer1[SEND_CHARS] = {'o'};


  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    while(1)
    {
      // change send_byte and/or send_buffer with what you want to send. Then:
      //RS232_SendByte(COMPORT, send_byte); // or:

      testMode();

      //RS232_SendBuf(comport, send_buffer, SEND_CHARS);
      // and/or:
      //RS232_PollComport(COMPORT, receive_buffer, RECEIVE_CHARS);

      // do something with received data in buffer
      // maybe sleep for a while
    }
    RS232_CloseComport(comport);
  }

  //RS232_CloseComport(comport);


  return 0;
}

void testMode()
{
  cout<<"Write buffer to send:"<<endl;
  unsigned char send_buffer[SEND_CHARS];
  cin >> send_buffer;
  RS232_SendBuf(comport, send_buffer, SEND_CHARS);
}

void cParser(int argN, char *argv[])
{
  int number;
  for(int i = 1; i < argN; i++)
  {
    number = stoi(string(argv[i]).substr(1).c_str(),nullptr);
    switch(argv[i][0])
    {
        case 't':
          test = true;
        break;

        case 'p':
          comport = number;
        break;

        case 'b':
          baudrate = number;
        break;

        default:
        break;
    }
  }
}
