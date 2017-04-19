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
//#include <stdlib.h>

void inputMotion(byte ReceivedData);

	extern volatile bool new_command;             // current command
	extern volatile uint8 bioloid_command;                // current command
	extern volatile uint8 last_bioloid_command;   // last command
	extern volatile uint8 current_motion_page;   // last command
	extern volatile uint8 current_step;

	int16 last_pose[NUM_AX12_SERVOS];


int main(void) {

	// Must include this line in all programs using "cm530.h/.c"
	SysInit();

	/* External vars for the motion state machine state */
	byte	ReceivedData = 0, oldReceivedData = 0;
	char    tmpComm[128];
	char * ReceivedCommand;
	ReceivedCommand = tmpComm;
	PrintString("Starting Program by turning on POWER LED!\r\n");
	SetLED(POWER, 1);
	executeMotion(MOTION_STAND);

	while(1){

		Battery_Monitor_Alarm();

		update_servo_positions();
		gyro_update();



		ReceivedData = std_getchar();
		if(ReceivedData != oldReceivedData )
			inputMotion(ReceivedData);

		oldReceivedData = ReceivedData;

		executeMotionSequence();

		//apply_new_pose_and_offsets();
		//mDelay(100);
	}

    PrintString("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    PrintString("CM-530 Experimental Example         ");
    //Buzzed(150, 2300);    // 217 Hz ~ A_4

}

void inputMotion(byte ReceivedData){
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
				}*/
				else if(ReceivedData == 'b'){
					PrintString("\nPlaying Some music\n");
					Buzzed(150, 200);    // 2500 Hz ~ Ds_7/Eb_7
				}
				else if(ReceivedData == 's'){
					bioloid_command = COMMAND_STOP;

				}
			// just testing
				else if(ReceivedData == 't'){
					executeMotion(26); // stand up
					mDelay(2000);
					executeMotion(7);
					mDelay(2000);
					executeMotion(25); // sit down again
				}
				else if(ReceivedData == 'w'){
					startMotionIfIdle(MOTION_STAND);
					startMotionIfIdle(MOTION_START_WALK);
				}
				else if(ReceivedData == 'a'){
					startMotionIfIdle(MOTION_STAND);

				}
				else if(ReceivedData == 'g'){
					startMotionIfIdle(MOTION_SIT);
				}
		/*
		 * DONT NOw what this is
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

void _exit(void) {
    while(1) {
        // Loop until reset
    }
}

