#include "comm.h"
#include "rs232.h"

int comm_OpenComport(int comport_number, int baudrate)
{
  return RS232_OpenComport(comport_number, baudrate, "8N1");
}
int comm_PollComport(int comport_number, unsigned char *buf, int size)
{
  return RS232_PollComport(comport_number, buf, size);
}
int comm_SendByte(int comport_number, unsigned char byte)
{
  return RS232_SendByte(comport_number, byte);
}
int comm_SendBuf(int comport_number, unsigned char *buf, int size)
{
  return RS232_SendBuf(comport_number, buf, size);
}
void comm_CloseComport(int comport_number)
{
  RS232_CloseComport(comport_number);
}
