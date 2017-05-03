#include "rs232.h"


#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

void manualMode();

#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      10      // or whatever size of buffer you want to send

int comport = 16; //pNUMBER
int baudrate = 57600; //bNUMBER

//REMOVE THESE LATER
unsigned char receive_buffer[RECEIVE_CHARS];
unsigned char send_byte = 42;
unsigned char send_buffer[SEND_CHARS];

/*
Usage:
To change running parameters use add the arguments described above when running the program.
Example: ./main p123 sets the comport to 123
*/
int main (int argc, char *argv[]) {
  //Set up:

  cout<<"The program is running these values:"<<endl;
  cout<<"Comport: "<<comport<<endl;
  cout<<"Baudrate: "<<baudrate<<endl;
  manualMode();
  return 0;
}


void manualMode()
{
  if(RS232_OpenComport(comport, baudrate, "8N1") != 1)
  {
    int c;
    while(1)
    {
      cout<<"Tests:"<<endl;
      cout<<"1 - Write buffer"<<endl;
      cout<<"2 - Write byte"<<endl;
      cout<<"0 - Exit"<<endl;
      c = 0;
      cin.clear();
      cin>>c;
      if(!cin.fail())
      {
        switch(c)
        {
          case 0:
            return;
          break;
          case 1:
            while(1)
            {
              cout<<"Write buffer to send:"<<endl;
              cin.clear();
              cin >> send_buffer;
              if(send_buffer[0] == '0') {break;}
              RS232_SendBuf(comport, send_buffer, SEND_CHARS);
            }
          break;
          case 2:
            cout<<"Not implemented"<<endl;
            /*
            while(1)
            {
            cout<<"Write buffer to send:"<<endl;
            cin.clear();
            cin >> send_byte;
            if(send_byte == '0') {break;}
            RS232_SendBuf(comport, send_byte);
            }*/
        break;
        default:
          cout<<"Please provide proper input"<<endl;
        break;
      }
    }
    else
    {
      cout<<"Please provide proper input"<<endl;
    }
    /*
    cout<<"Respone:"<<endl;
    RS232_PollComport(comport, receive_buffer, RECEIVE_CHARS);
    cout<<receive_buffer<<endl; */
    }
    RS232_CloseComport(comport);
  }
}
