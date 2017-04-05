#include "rs232.h"

#define COMPORT         5     // this is '/dev/ttyUSB0' (for arduino) or choose wherever you added '/dev/ttyAMA0' (for raspberry pi UART pins) to the list
#define BAUDRATE        57600     // or whatever baudrate you want
#define RECEIVE_CHARS   8      // or whatever size of buffer you want to receive
#define SEND_CHARS      1      // or whatever size of buffer you want to send

int main (int argc, char **argv) {
  unsigned char receive_buffer[RECEIVE_CHARS];
  unsigned char send_byte = 42;
  unsigned char send_buffer[SEND_CHARS] = {'a','b','c','d'};

  OpenComport(COMPORT, BAUDRATE);
  while(1) {

    cout<<"Write command: "<<endl;
    cin>>send_buffer[0];

    cout<<send_buffer[0]<<endl;
    system("pause");

    // change send_byte and/or send_buffer with what you want to send. Then:
    SendByte(COMPORT, send_byte); // or:
    SendBuf(COMPORT, send_buffer, SEND_CHARS);

    // and/or:
    PollComport(COMPORT, receive_buffer, RECEIVE_CHARS);

    // do something with received data in buffer
    // maybe sleep for a while
  }
  CloseComport(COMPORT);

  return 0;
}
