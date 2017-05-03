#include "rs232.h"

void manualMode(int, int);
int comm_OpenComport(int comport, int boudrate, const char * config);
int comm_PollComport(int comport, unsigned char *receive_buffer, int receive_chars);
//int RS232_SendByte(int comport, unsigned char byte);
int comm_SendBuf(int comport, unsigned char *send_buffer, int send_chars);
void comm_CloseComport(int comport);
