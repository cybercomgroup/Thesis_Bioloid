#include "rs232.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      10      // or whatever size of buffer you want to send

void manualMode();

//Parser and global belonging variables
void cParser(int argN, char *argv[]);

bool test = false;
int comport = 16; //pNUMBER
int baudrate = 57600; //bNUMBER

/*
Usage:
To change running parameters use add the arguments described above when running the program.
Example: ./main p123 sets the comport to 123
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
      if(test)
      {
        manualMode();
      }
      else
      {

      }

      // change send_byte and/or send_buffer with what you want to send. Then:
      //RS232_SendByte(COMPORT, send_byte); // or:


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

void manualMode()
{
  int c = 0;
  unsigned char send_buffer[SEND_CHARS];
  unsigned char send_byte = 42;
  while(1)
  {
    cout<<"Tests:"<<endl;
    cout<<"1 - Write buffer"<<endl;
    cout<<"2 - Write byte"<<endl;
    cin>>c;
    if(!cin.fail() || c > 3)
    {
      switch(c)
      {
        case 1:
          cout<<"Write buffer to send:"<<endl;
          cin >> send_buffer;
          RS232_SendBuf(comport, send_buffer, SEND_CHARS);
        break;
        case 2:
          cout<<"Write byte to send:"<<endl;
          cin >> send_buffer;
          RS232_SendByte(comport, send_byte);
        break;
        default:
        break;
      }
    }
    else if(c==3)
    {
      return;
    }
    else
    {
      cout<<"Please provide proper input"<<endl;
    }
  }
}

void cParser(int argN, char *argv[])
{
  int number;
  string input;
  for(int i = 1; i < argN; i++)
  {
    input = argv[i];
    if(input.length() > 1)
    {
      number = stoi(string(argv[i]).substr(1).c_str(),nullptr);
    }
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
