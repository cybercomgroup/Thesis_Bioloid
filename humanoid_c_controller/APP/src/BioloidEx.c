/*
 * BioloidEx.c
 *
 *  Created on: 8 Jun 2016
 *      Author: E1193262
 */


#include "BioloidEx.h"
#include "dynamixel_address_tables.h"
#include "dynamixel.h"
#include "serial.h"
#include "wiring.h"
#include "system_func.h"

unsigned char interpolating;          // are we in an interpolation? 0=No, 1=Yes
unsigned char runningSeq;              // are we running a sequence? 0=No, 1=Yes
int poseSize;                // how many servos are in this pose, used by Sync()
u8 frameLength;                 // Allow variable frame lengths, to test...
unsigned int pose_[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned int nextpose_[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0 };
int speed_[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned char id_[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//    unsigned long lastframe_;                   // time last frame was sent out
unsigned long nextframe_;                   //
transition_t * sequence;                    // sequence we are running
int transitions;                    // how many transitions we have left to load


//  initializes serial1 transmit at baud, 8-N-1
void BioloidControllerEx() {
	int i;
	// setup storage
	/**id_ = (unsigned char *) malloc(AX12_MAX_SERVOS * sizeof(unsigned char));
	 pose_ = (unsigned int *) malloc(AX12_MAX_SERVOS * sizeof(unsigned int));
	 nextpose_ = (unsigned int *) malloc(AX12_MAX_SERVOS * sizeof(unsigned int));
	 speed_ = (int *) malloc(AX12_MAX_SERVOS * sizeof(int));*/
	// initialize
	for (i = 0; i < AX12_MAX_SERVOS; i++) {
		id_[i] = i + 1;
		pose_[i] = 512;
		nextpose_[i] = 512;
	}
	frameLength = BIOLOID_FRAME_LENGTH;
	interpolating = 0;
	nextframe_ = getMillis();
//#ifdef MILLIS
//	PrintString("BioloidEX nextframe_ ");
//	Printu32d(nextframe_);
//	PrintString("\n");
//#endif
}

/* new-style setup */
void Bioloid_Setup(int servo_cnt) {
	int i;
	// setup storage
	/**id_ = (unsigned char *) malloc(servo_cnt * sizeof(unsigned char));
	 pose_ = (unsigned int *) malloc(servo_cnt * sizeof(unsigned int));
	 nextpose_ = (unsigned int *) malloc(servo_cnt * sizeof(unsigned int));
	 speed_ = (int *) malloc(servo_cnt * sizeof(int));*/
	// initialize
	poseSize = servo_cnt;
	for (i = 0; i < poseSize; i++) {
		id_[i] = i + 1;
		pose_[i] = 512;
		nextpose_[i] = 512;
	}
	interpolating = 0;
	nextframe_ = getMillis();
#ifdef MILLIS
	PrintString("BioloidEX nextframe_ ");
	Printu32d(nextframe_);
	PrintString("\n");
#endif
}
void BioloidControllerEx_setId(int index, int id) {
	id_[index] = id;
}
int BioloidControllerEx_getId(int index) {
	return id_[index];
}

/* load a named pose from FLASH into nextpose. */
void BioloidControllerEx_loadPose(const unsigned int * addr) {
	int i;
	poseSize = pgm_read_word_near(addr); // number of servos in this pose
	for (i = 0; i < poseSize; i++)
		//nextpose_[i] = pgm_read_word_near(addr+1+i) << BIOLOID_SHIFT;
		nextpose_[i] = pgm_read_word_near(addr + 1 + i);
}
/* read in current servo positions to the pose. */
void BioloidControllerEx_readPose() {
	int i;

#ifdef DEBUG_BIOLOIDEX
	PrintString("Bioloid controller --- READ POSE ---\n");
	PrintString("ID - POSICION\n");
#endif
	for (i = 0; i < poseSize; i++) {
		//pose_[i] = dxl_read_word(id_[i],AXM_PRESENT_POSITION_L)<<BIOLOID_SHIFT;
		pose_[i] = dxl_read_word(id_[i], AXM_PRESENT_POSITION_L);

#ifdef DEBUG_BIOLOIDEX
		PrintChar(id_[i]);
		PrintString(" - ");
		TxD_Dec_U16(pose_[i]);
		PrintString("\n");
#endif
		mDelay(25);
	}
}
/* write pose out to servos using sync write. */
void BioloidControllerEx_writePose() {
	int temp;
	dxl_set_txpacket_id (BROADCAST_ID);
	dxl_set_txpacket_instruction (INST_SYNC_WRITE);
	dxl_set_txpacket_parameter(0, AXM_GOAL_POSITION_L);
	dxl_set_txpacket_parameter(1, 2);
	int i;
	for (i = 0; i < poseSize; i++) {
		//temp = pose_[i] >> BIOLOID_SHIFT;
		temp = pose_[i];
		dxl_set_txpacket_parameter(2 + 3 * i, id_[i]);
		dxl_set_txpacket_parameter(2 + 3 * i + 1, dxl_get_lowbyte(temp));
		dxl_set_txpacket_parameter(2 + 3 * i + 2, dxl_get_highbyte(temp));
	}
	dxl_set_txpacket_length((2 + 1) * poseSize + 4);
	dxl_txrx_packet();
	u16 CommStatus = dxl_get_result();
#ifdef USING_PC_UART
	if (CommStatus == DXL_RXSUCCESS)
		PrintErrorCode();
	else
		PrintCommStatus(CommStatus);
#endif
}

/* set up for an interpolation from pose to nextpose over TIME
 milliseconds by setting servo speeds. */
void BioloidControllerEx_interpolateSetup(int time) {
	int i;
	int frames = (time/frameLength) + 1;
	nextframe_ = getMillis() + frameLength;
#ifdef MILLIS
	PrintString("BioloidEX BioloidControllerEx_interpolateSetup frames ");
	Printu32d(frames);
	PrintString("\n");
	PrintString("BioloidEX BioloidControllerEx_interpolateSetup nextframe_ ");
	Printu32d(nextframe_);
	PrintString("\n");
#endif
	// set speed each servo...
	for (i = 0; i < poseSize; i++) {
		if (nextpose_[i] > pose_[i]) {
			speed_[i] = (nextpose_[i] - pose_[i]) / frames + 1;
		} else {
			speed_[i] = (pose_[i] - nextpose_[i]) / frames + 1;
		}
#ifdef DEBUG_BIOLOIDEX
		PrintString("Bioloid controller --- INTERPOLATE SETUP ---\n");
		PrintString("ID - SPEED\n");
		PrintChar(id_[i]);
		PrintString(" - ");
		TxD_Dec_U16(speed_[i]);
		PrintString("\n");
#endif
	}
	interpolating = 1;
}
/* interpolate our pose, this should be called at about 30Hz. */
#define WAIT_SLOP_FACTOR 10
int BioloidControllerEx_interpolateStep(bool fWait) {

	if (interpolating == 0)
		return 0;
	int i;
	int complete = poseSize;
	if (!fWait) {
		if (getMillis() < (nextframe_ - WAIT_SLOP_FACTOR)) {
			return (getMillis() - nextframe_); // We still have some time to do something...
		}
	}
#ifdef MILLIS
	PrintString("BioloidEX  BioloidControllerEx_interpolateStep nextframe_ ");
	Printu32d(nextframe_);
	PrintString("\n");
#endif
	while (getMillis() < nextframe_);
#ifdef MILLIS
	PrintString("BioloidEX  BioloidControllerEx_interpolateStep nextframe_-Millis ");
	Printu32d(getMillis());
	PrintString("\n");
#endif

	nextframe_ = getMillis() + frameLength;
#ifdef MILLIS
	PrintString("BioloidEX  BioloidControllerEx_interpolateStep nextframe_ (Millis + framelength) ");
	Printu32d(nextframe_);
	PrintString("\n");
#endif
	// update each servo
	for (i = 0; i < poseSize; i++) {
		int diff = nextpose_[i] - pose_[i];
		if (diff == 0) {
			complete--;
		} else {
			if (diff > 0) {
				if (diff < speed_[i]) {
					pose_[i] = nextpose_[i];
					complete--;
				} else
					pose_[i] += speed_[i];
			} else {
				if ((-diff) < speed_[i]) {
					pose_[i] = nextpose_[i];
					complete--;
				} else
					pose_[i] -= speed_[i];
			}
		}

#ifdef DEBUG_BIOLOIDEX
		PrintString("Bioloid controller --- INTERPOLATE STEP ---\n");
		PrintString("ID - POSE\n");
		PrintChar(id_[i]);
		PrintString(" - ");
		TxD_Dec_U16(pose_[i]);
		PrintString("\n");
#endif

	}
	if (complete <= 0)
		interpolating = 0;
	BioloidControllerEx_writePose();
	return 0;
}

/* get a servo value in the current pose */
int BioloidControllerEx_getCurPose(int id) {
	int i;
	for (i = 0; i < poseSize; i++) {
		if (id_[i] == id)
			//return ((pose_[i]) >> BIOLOID_SHIFT);
			return ((pose_[i]));
	}
	return -1;
}
/* get a servo value in the next pose */
int BioloidControllerEx_getNextPose(int id) {
	int i;
	for (i = 0; i < poseSize; i++) {
		if (id_[i] == id)
			//return ((nextpose_[i]) >> BIOLOID_SHIFT);
			return ((nextpose_[i]));
	}
	return -1;
}
/* set a servo value in the next pose */
void BioloidControllerEx_setNextPose(int id, int pos) {
	int i;
	for (i = 0; i < poseSize; i++) {
		if (id_[i] == id) {
			//nextpose_[i] = (pos << BIOLOID_SHIFT);
			nextpose_[i] = (pos);
			return;
		}
	}
}

/* Added by Kurt */
void BioloidControllerEx_setNextPoseByIndex(int index, int pos) { // set a servo value by index for next pose
	if (index < poseSize) {
		//nextpose_[index] = (pos << BIOLOID_SHIFT);
		nextpose_[index] = (pos);
	}
}
