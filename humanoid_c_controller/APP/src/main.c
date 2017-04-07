#define HEXMODE
//#define MILLIS

/*
 * main.cpp
 *
 *  Created on: 17 jan. 2017
 *      Author: miali1
 */
#include "system_init.h"
#include "led.h"
#include "stm32f10x_lib.h"
#include "dynamixel_address_tables.h"
#include "serial.h"
#include "usart.h"
#include "wiring.h"
#include "adc.h"
#include "button.h"
#include "mic.h"
//#include <stdlib.h>

int main(void) {

	// Must include this line in all programs using "cm530.h/.c"
	SysInit();

	byte	ReceivedData;
	char    tmpComm[128];
	char * ReceivedCommand;
	ReceivedCommand = tmpComm;
	PrintString("Starting Program by turning on POWER LED!\r\n");
	SetLED(POWER, 1);

	while(1){


		PrintString("Battery Voltage: ");
		Printu32d((u32)ReadAnalog(VBUS)>>4);
		PrintString("e-1 [Volts]\n");

		Battery_Monitor_Alarm();

		    PrintString("PCU:");
		#ifdef USING_PC_UART
		    Printu32d(Baudrate_PCU);
		    PrintString("(bps)\n");
		#else
		    PrintString("Not in use\n");
		#endif

		    PrintString("ZIG:");
		#ifdef USING_ZIGBEE
		    Printu32d(Baudrate_ZIG);
		    PrintString("(bps)\n");
		#else
		    PrintString("Not in use\n");
		#endif

		    PrintString("DXL:");
		#ifdef USING_DYNAMIXEL
		    Printu32d(Baudrate_DXL);
		    PrintString("(bps)\n");
		#else
		    PrintString("Not in use\n");
		#endif

//		Buzzed(150, 200);    // 2500 Hz ~ Ds_7/Eb_7


		ReceivedData = std_getchar();;

		//if(strcmp(ReceivedCommand, "led on") == 0){
		if(ReceivedData == 'l'){
			std_putchar(ReceivedData);
			PrintString(" * \t* Turn on LEDs!\r\n");
			SetLED(POWER, 1);
			SetLED(PLAY, 1);
			SetLED(MANAGE, 1);
			SetLED(PROGRAM, 1);
		}
		else if(ReceivedData == 'o'){
		//else if(strcmp(ReceivedCommand, "led off")){
			std_putchar(ReceivedData);
			PrintString(" * \t* Turn off LEDs!\r\n");
			SetLED(POWER, 0);
			SetLED(PLAY, 0);
			SetLED(MANAGE, 0);
			SetLED(PROGRAM, 0);
		}
		//else if(strcmp(ReceivedCommand, "motors report")){
		else if(ReceivedData == 'm'){
			u8 id, num=0;
			u16 wdata, error, pdata, perror;
			dxl_initialize(Baudrate_DXL);
			for (id=1; id<(250); id++)
			{
				wdata = (dxl_read_byte(id, P_ID)&0x00FF);
				if (wdata==id)
				{
					wdata=0;
					num++;
					PrintString("{");
					Printu32d(id);
					PrintString(", ");

					// Read present position
					wdata = dxl_read_word(id, P_MODEL_NUMBER_L);
					error = dxl_get_result();


					if (!(error&DXL_RXSUCCESS))
						PrintCommStatus(error);
					else{
						PrintString(",* ");
						Printu32d(dxl_read_word( id, AXM_PRESENT_POSITION_L ));
						PrintString("*, ");
					}
					Printu32d(wdata);
					if (wdata==MODEL_AX12)
					{
						PrintString(" (AX-12)");
					}
					else if (wdata==MODEL_AX18)
					{
						PrintString(" (AX-18)");
					}
					else if (wdata==MODEL_AXS1)
					{
						PrintString(" (AX-S1)");
					}
					else if (wdata==MODEL_AXS20)
					{
						PrintString(" (AX-S20)");
					}
					else if (wdata==MODEL_JHFPS)
					{
						PrintString(" (JH-FPS)");
					}
					else if (wdata==MODEL_MX28)
					{
						PrintString(" (MX-28)");
					}

					PrintString(", ");
					Printu32d(dxl_read_byte(id, P_FIRMWARE_VERSION));

					PrintString("} \n");
				}
			}
			PrintString("\nDXL DEVICES:");
			Printu32d(num);
			PrintString("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		}
		else if(ReceivedData == 'b'){
			PrintString("\nPlaying Some music\n");
			Buzzed(150, 200);    // 2500 Hz ~ Ds_7/Eb_7
		}
		else if(ReceivedData == 'i'){
			PrintString("\n(IR_L, IR_R, DMS):\t(");
			Prints32d(ReadIR(EPORT1A));
			//Printu32d(ReadIR(EPORT1A));
			PrintString(", ");
			Prints32d(ReadIR(EPORT6A));
			PrintString(", ");
			Prints32d(ReadIR(EPORT5A));
			PrintString(")\n");
			PlayNote(300, NOTE_E, 1);    // 2500 Hz ~ Ds_7/Eb_7
			mDelay(2000);
		}
		else if(ReadButton(UP)){
			PrintString("\nUP UP UP is pressed!\n");
			//Buzzed(150, 200);    // 2500 Hz ~ Ds_7/Eb_7
		}
		else if(ReadButton(DOWN)){
			PrintString("\nDOWN DOWN DOWN is pressed!\n");
		}
		else if(ReadButton(LEFT)){
			PrintString("\nLEFT LEFT LEFT is pressed!\n");
		}
		else if(ReadButton(RIGHT)){
			PrintString("\nRIGHT RIGHT RIGHT is pressed!\n");
		}
		else if(ReadButton(START)){
			PrintString("\nSTART START START is pressed!\n");
			PlayNote(200, NOTE_A, 1);    // 2500 Hz ~ Ds_7/Eb_7
		}
		else if(ReadButton(MIC)){
			PrintString("\nMIC MIC MIC MIC MIC MIMIC MIC MIMIC MIC MIMIC MIC MIMIC MIC MIMIC MIC MIMIC MIC MIMIC MIC MIMIC MIC MIMIC MIC MIis pressed!\n");
			SetLED(MANAGE, 1);
			PlayNote(200, NOTE_A, 1);    // 2500 Hz ~ Ds_7/Eb_7
			//mDelay(300);
			//SetLED(MANAGE, 0);
			//mDelay(2000);
		}
		else{
			PrintString("\nWait for command\n");
//			ReceivedCommand = std_gets(ReceivedCommand);
//			if( ReceivedCommand != 0){
//				PrintString("\nThe entered string is: \t");
//				std_puts(ReceivedCommand);
//				PrintString("\n");
//			}
		}
		//mDelay(1000);
		mDelay(100);
	}

    PrintString("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    PrintString("CM-530 Experimental Example         ");
    //Buzzed(150, 2300);    // 217 Hz ~ A_4

}

