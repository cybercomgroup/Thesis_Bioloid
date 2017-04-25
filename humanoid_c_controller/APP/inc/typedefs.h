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

// Outputs
#define OUTPUT_STOP				'b'
#define OUTPUT_OK				'g'
// Inputs
#define INPUT_WALK_F			'w'
#define INPUT_WALK_B			's'
#define INPUT_TURN_L			'a'
#define INPUT_TURN_R			'd'
#define INPUT_RAPCHEST			'c'
#define INPUT_SIT				'g'
#define INPUT_STAND				'u'
#define INPUT_SOUND				'p'
#define INPUT_KICK_L			'h'
#define INPUT_KICK_R			'k'


#endif /* APP_INC_TYPEDEFS_H_ */
