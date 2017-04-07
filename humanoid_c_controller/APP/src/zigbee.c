/*
 * zigbee.c
 *
 *  Created on: 9 Jun 2016
 *      Author: E1193262
 */


#include "zigbee.h"

u8 gbRcvPacket[PACKET_LENGTH];
u8 gbRcvPacketArduino[PACKET_LENGTH_ARDUINO];
u8 gbRcvPacketNum;
u16 gwRcvData;
volatile u8 gbRcvFlag;

u8 right_V;
u8 right_H;
u8 left_V;
u8 left_H;
u8 buttons_;
u8 extra;


//##############################################################################
u8 zgb_initialize(u32 baudrate)
{
	if (zgb_hal_open(baudrate)==0)
	return 0;

	gbRcvFlag = 0;
	gwRcvData = 0;
	gbRcvPacketNum = 0;
	return 1;
}

//##############################################################################
void zgb_terminate(void)
{
	zgb_hal_close();
}

//##############################################################################
u8 zgb_tx_data(u16 word)
{
	u8 SndPacket[6];
	u8 lowbyte = (u8) (word&0xFF);
	u8 highbyte = (u8) ((word>>8)&0xFF);

	SndPacket[0] = 0xFF;
	SndPacket[1] = 0x55;
	SndPacket[2] = lowbyte;
	SndPacket[3] = ~lowbyte;
	SndPacket[4] = highbyte;
	SndPacket[5] = ~highbyte;

	if (zgb_hal_tx(SndPacket, 6)!=6)
	return 0;

	return 1;
}

//##############################################################################
u8 zgb_rx_check(void)
{
	u8 RcvNum;
	u8 checksum;
	u8 i, j;

	if (gbRcvFlag==1)
	return 1;

	// Fill packet buffer
	if (gbRcvPacketNum<6)
	{
		RcvNum = zgb_hal_rx((u8*)&gbRcvPacket[gbRcvPacketNum], (6-gbRcvPacketNum));
		if (RcvNum!=-1)
		gbRcvPacketNum += RcvNum;
	}

	// Find header
	if (gbRcvPacketNum>=2)
	{
		for (i=0; i<gbRcvPacketNum; i++)
		{
			if (gbRcvPacket[i]==0xFF)
			{
				if (i<=(gbRcvPacketNum-2))
				{
					if (gbRcvPacket[i+1]==0x55)
					break;
				}
			}
		}

		if (i>0)
		{
			if (i==gbRcvPacketNum)
			{
				// Cannot find header
				if (gbRcvPacket[i-1]==0xFF)
				i--;
			}

			// Remove data before header
			for (j=i; j<gbRcvPacketNum; j++)
			{
				gbRcvPacket[j-i] = gbRcvPacket[j];
			}
			gbRcvPacketNum -= i;
		}
	}

	// Verify packet
	if (gbRcvPacketNum==6)
	{
		if ( (gbRcvPacket[0]==0xFF) && (gbRcvPacket[1]==0x55) )
		{
			checksum = ~gbRcvPacket[3];
			if (gbRcvPacket[2]==checksum)
			{
				checksum = ~gbRcvPacket[5];
				if (gbRcvPacket[4]==checksum)
				{
					gwRcvData = (u16) ((gbRcvPacket[4]<<8)&0xFF00);
					gwRcvData += gbRcvPacket[2];
					gbRcvFlag = 1;
				}
			}
		}
		gbRcvPacket[0] = 0x00;
		gbRcvPacketNum = 0;
	}

	return gbRcvFlag;
}

//##############################################################################
u16 zgb_rx_data(void)
{
	gbRcvFlag = 0;
	return gwRcvData;
}

//##############################################################################
u8 zgb_rx_check_arduino(void)
{


    u8 RcvNum;
    u8 checksum;
    u8 i, j;

    if (gbRcvFlag==1){
    	//PrintString("\n NO \n");
    	return 1;
    }



    // Fill packet buffer
    if (gbRcvPacketNum<9)


    {


        RcvNum = zgb_hal_rx((u8*)&gbRcvPacketArduino[gbRcvPacketNum], (9-gbRcvPacketNum));
        if (RcvNum!=-1)
            gbRcvPacketNum += RcvNum;
    }

    // Find header
    if (gbRcvPacketNum>=2)
    {

        for (i=0; i<gbRcvPacketNum; i++)
        {
            if (gbRcvPacketArduino[i]==0xFF)
            {
                if (i<=(gbRcvPacketNum-2))
                {
                    if (gbRcvPacketArduino[i+1]==0x55)
                        break;
                }
            }
        }

        if (i>0)
        {
            if (i==gbRcvPacketNum)
            {
                // Cannot find header
                if (gbRcvPacketArduino[i-1]==0xFF)

                    i--;
            }

            // Remove data before header
            for (j=i; j<gbRcvPacketNum; j++)
            {

                gbRcvPacketArduino[j-i] = gbRcvPacketArduino[j];
            }
            gbRcvPacketNum -= i;
        }
    }

    // Verify packet
    if (gbRcvPacketNum==9)
    {

        if ( (gbRcvPacketArduino[0]==0xFF) && (gbRcvPacketArduino[1]==0x55) )
        {
            /**checksum = ~gbRcvPacket[3];
            if (gbRcvPacket[2]==checksum)
            {
                checksum = ~gbRcvPacket[5];
                if (gbRcvPacket[4]==checksum)
                {
                    gwRcvData = (u16) ((gbRcvPacket[4]<<8)&0xFF00);
                    gwRcvData += gbRcvPacket[2];
                    gbRcvFlag = 1;
                }
            }*/


                  		left_V  =   gbRcvPacketArduino[2];
        	            left_H  =   gbRcvPacketArduino[3];
        	            right_V =   gbRcvPacketArduino[4];
        	            right_H =   gbRcvPacketArduino[5];
        	            buttons_=   gbRcvPacketArduino[6];
        	            extra   =   gbRcvPacketArduino[7];
        	            checksum=   gbRcvPacketArduino[8];
#ifdef DEBUG_ZIGBEE
	PrintString("ZIGBEE --- zgb_rx_check_arduino ---\n");
	PrintString("rightV rightH leftV leftH buttons ext\n");
	TxD_Dec_U8(right_V);
	PrintString(" ");
	TxD_Dec_U8(right_H);
	PrintString(" ");
	TxD_Dec_U8(left_V);
	PrintString(" ");
	TxD_Dec_U8(left_H);
	PrintString(" ");
	TxD_Dec_U8(buttons_);
	PrintString(" ");
	TxD_Dec_U8(extra);
	PrintString("\n");
#endif


            if (checksum == (255 - (right_V+right_H+left_V+left_H+buttons_)%256)){        	  //if (checksum == (255 - (gbRcvPacketArduino[2]+gbRcvPacketArduino[3]+gbRcvPacketArduino[4]+gbRcvPacketArduino[5]+gbRcvPacketArduino[6])%256)){


				 gbRcvFlag = 1;
            }

        }
        gbRcvPacketArduino[0] = 0x00;
        gbRcvPacketNum = 0;
    }

    return gbRcvFlag;
}

//##############################################################################
u16 zgb_rx_data_right_V(void)
{
    //gbRcvFlag = 0;
    return right_V;
}
//##############################################################################
u16 zgb_rx_data_right_H(void)
{
    //gbRcvFlag = 0;
    return right_H;
}
//##############################################################################
u16 zgb_rx_data_left_V(void)
{
    //gbRcvFlag = 0;
    return left_V;
}
//##############################################################################
u16 zgb_rx_data_left_H(void)
{
    //gbRcvFlag = 0;
    return left_H;
}
//##############################################################################
u8 zgb_rx_data_buttons(void)
{
    //gbRcvFlag = 0;
    return buttons_;
}
//##############################################################################
u8 zgb_rx_data_extra(void)
{
    gbRcvFlag = 0;
    return 0;

}
//##############################################################################
int zgb_rx_data_right_V_(void)
{
    //gbRcvFlag = 0;
    return (right_V - 128);
}
//##############################################################################
int zgb_rx_data_right_H_(void)
{
    //gbRcvFlag = 0;
    return (right_H - 128);
}
//##############################################################################
int zgb_rx_data_left_V_(void)
{
    //gbRcvFlag = 0;
    return (left_V - 128);
}
//##############################################################################
int zgb_rx_data_left_H_(void)
{
    //gbRcvFlag = 0;
    return (left_H- 128);
}
//##############################################################################
byte CheckZBNewArrive(void) {
	if (gbZigRead != gbZigWrite)
		return 1;
	else
		return 0;
}
