#ifndef comm_INCLUDED
#define comm_INCLUDED

#include <iostream>
#include <string>


extern "C" {

#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/file.h>
#include <errno.h>

int comm_OpenComport(int, int);
int comm_PollComport(int, unsigned char *, int);
int comm_SendByte(int, unsigned char);
int comm_SendBuf(int, unsigned char *, int);
void comm_CloseComport(int);

} /* extern "C" */

#endif
