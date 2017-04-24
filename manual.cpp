#include "rs232.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

void main()
{
  int c = 0;
  unsigned char send_buffer[SEND_CHARS];
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
          RS232_SendByte(comport, send_buffer, SEND_CHARS);
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
