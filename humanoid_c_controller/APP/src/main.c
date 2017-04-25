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
#include "motion_f.h"
#include "pose.h"
#include "walk.h"
#include "sensors.h"
//#include <stdlib.h>

void update_servo_positions();
void TxDWord16(u16 wSentData);
void TxDByte16(u8 bSentData);
void TxDByte_PC(u8 bTxdData);


void inputMotion(byte ReceivedData);

	           // current command

	extern volatile uint8 bioloid_command;  // current command
	/* This is not in use right now
	extern volatile uint8 last_bioloid_command;   // last command
	extern volatile uint8 current_motion_page;   // last command
	extern volatile uint8 current_step;
	extern volatile bool new_command;
*/
	int16 last_pose[NUM_AX12_SERVOS];


int main(void) {

	// Must include this line in all programs using "cm530.h/.c"
	SysInit();

	/* External vars for the motion state machine state */
	byte	ReceivedData = 0, oldReceivedData = 0;
	u16 ir_test;
	char    tmpComm[128];
	char * ReceivedCommand;
	ReceivedCommand = tmpComm;
	PrintString("Starting Program by turning on POWER LED!\r\n");
	SetLED(POWER, 1);
	executeMotion(MOTION_STAND);

	while(1){
		//We do not use a battery at this point
	//	Battery_Monitor_Alarm();

		update_servo_positions();
		gyro_update();

		ir_test = read_ir_mid();
		//mDelay(1000);
		if(ir_test > 0x44C){
			PrintString("\n New value: \n");
			TxDWord16(ir_test);
		}
		ReceivedData = std_getchar();
		if(ReceivedData != oldReceivedData )
			inputMotion(ReceivedData);

		oldReceivedData = ReceivedData;

		executeMotionSequence();
		////only use if  SYNC pose mode is enable, we will never use this
		//apply_new_pose_and_offsets();
		//mDelay(100);
	}

    PrintString("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    PrintString("CM-530 Experimental Example         ");
    //Buzzed(150, 2300);    // 217 Hz ~ A_4

}

void inputMotion(byte ReceivedData){

		switch(ReceivedData){

		case 'l':
			PrintString(" * \t* Turn on LEDs!\r\n");
			SetLED(POWER, 1);
			SetLED(PLAY, 1);
			SetLED(MANAGE, 1);
			SetLED(PROGRAM, 1);
			break;

		case 'o':
			PrintString(" * \t* Turn off LEDs!\r\n");
			SetLED(POWER, 0);
			SetLED(PLAY, 0);
			SetLED(MANAGE, 0);
			SetLED(PROGRAM, 0);
			break;

		case 'p':
			PrintString("\nPlaying Some music\n");
			Buzzed(150, 200);    // 2500 Hz ~ Ds_7/Eb_7
			break;

		case 'b':
			bioloid_command = COMMAND_STOP;
			break;

		case 'w':
			startMotionIfIdle(MOTION_WALK_FORWARD);
			break;

		case 's':
			startMotionIfIdle(MOTION_WALK_BACkWARD);
			break;

		case 'a':
			startMotionIfIdle(MOTION_TURN_LEFT);
			break;

		case 'd':
			startMotionIfIdle(MOTION_TURN_RIGHT);
			break;

		case 'g':
			startMotionIfIdle(MOTION_SIT);
			break;

		case 'u':
			startMotionIfIdle(MOTION_STAND);
			break;

		case 'c':
			startMotionIfIdle(MOTION_RAP_CHEST);
			break;

		case 'h':
			startMotionIfIdle(MOTION_KICK_L);
			break;

		case 'k':
			startMotionIfIdle(MOTION_KICK_R);
			break;


		}
		/*
		 * Do not know what this does
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

		  DONT NOw what this is
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
				}*/

}

int _servo_update_iteration;
// Update current servo positions.
// To save time:
// 		Check only servos that are expected to have moved.
void update_servo_positions() {
	if (!_servo_update_iteration)
	{
		// update half of servos
		for(int i=0; i<NUM_AX12_SERVOS/2; i++) {
			last_pose[i] = current_pose[i];
			current_pose[i] = dxl_read_word( i+1, DXL_PRESENT_POSITION_L );
		}
	}
	else
	{
		// update otehr half of servos
		for(int i=NUM_AX12_SERVOS/2; i<NUM_AX12_SERVOS; i++) {
			last_pose[i] = current_pose[i];
			current_pose[i] = dxl_read_word( i+1, DXL_PRESENT_POSITION_L );
		}
	}
	_servo_update_iteration = !_servo_update_iteration;
}
//TEST
void TxDWord16(u16 wSentData)
{
	TxDByte16((wSentData >> 8) & 0xff);
	TxDByte16(wSentData & 0xff);
}
void TxDByte16(u8 bSentData)
{
	byte bTmp;

	bTmp = ((byte) (bSentData >> 4) & 0x0f) + (byte) '0';
	if (bTmp > '9')
		bTmp += 7;
	TxDByte_PC(bTmp);
	bTmp = (byte) (bSentData & 0x0f) + (byte) '0';
	if (bTmp > '9')
		bTmp += 7;
	TxDByte_PC(bTmp);
}
void TxDByte_PC(u8 bTxdData)
{
	USART_SendData(USART3,bTxdData);
	while( USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET );
}



void _exit(void) {
    while(1) {
        // Loop until reset
    }
}

