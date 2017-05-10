#include "comm.h"
#include <iostream>

using namespace std;


#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      10      // or whatever size of buffer you want to send


unsigned char receive_buffer[RECEIVE_CHARS];
unsigned char send_byte = 42;
unsigned char send_buffer[SEND_CHARS];

void manualSend(int comport, int baudrate);

int main(int argc, char *argv[])
{
  manualSend(16,57600);
  return 0;
}

void manualSend(int comport, int baudrate)
{
  if(comm_OpenComport(comport, baudrate) != 1)
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
              comm_SendBuf(comport, send_buffer, SEND_CHARS);
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
    }
    comm_CloseComport(comport);
  }

}
