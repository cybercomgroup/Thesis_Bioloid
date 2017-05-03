#include "comm.h"


void manualMode(int comport, int baudrate)
{
  if(comm_OpenComport(comport, baudrate, "8N1") != 1)
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
    /*
    cout<<"Respone:"<<endl;
    RS232_PollComport(comport, receive_buffer, RECEIVE_CHARS);
    cout<<receive_buffer<<endl; */
    }
    comm_CloseComport(comport);
  }
}

int comm_OpenComport(int comport, int boudrate, const char * config)
{
  return RS232_OpenComport(comport, boudrate, config);
}
int comm_PollComport(int comport, unsigned char *receive_buffer, int receive_chars)
{
  return RS232_PollComport(int comport, unsigned char *receive_buffer, int receive_chars);
}
//int RS232_SendByte(int comport, unsigned char byte);
int comm_SendBuf(int comport, unsigned char *send_buffer, int send_chars)
{
  return RS232_SendBuf(comport,send_buffer,send_chars);
}
void comm_CloseComport(int comport)
{
  RS232_CloseComport(comport);
}
