/*
 * typedefs.h
 *
 *  Created on: 2 feb. 2017
 *      Author: install
 */

#ifndef APP_INC_TYPEDEFS_H_
#define APP_INC_TYPEDEFS_H_

#include <stdint.h>
#include "stm32f10x_type.h"

// Standard types
typedef uint8_t		uint8;
typedef int8_t		int8;
typedef uint16_t	uint16;
typedef int16_t		int16;
typedef uint32_t	uint32;
typedef int32_t		int32;


// Boolean types
#define FALSE ((bool)0)
#define TRUE  ((bool)1)


// COMMANDS OUT AND INPUTS TO AND FROM ROBOT
#define OUTPUT_COMMAND_STOP				'b'
#define INPUT_COMMAND_WALK_F			'w'
#define INPUT_COMMAND_WALK_B			's'
#define INPUT_COMMAND_TURN_L			'a'
#define INPUT_COMMAND_TURN_R			'd'
#define INPUT_COMMAND_RAPCHEST			'c'
#define INPUT_COMMAND_SIT				'g'
#define INPUT_COMMAND_STAND				'u'
#define INPUT_COMMAND_SOUND				'p'


#endif /* APP_INC_TYPEDEFS_H_ */
