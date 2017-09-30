/*
 *  CommonIncludes.h
 *
 *  Created by Dmitriy Solomonov on 12/21/10.
 *
 */

#ifndef _COMMON_INCLUDES_H_
#define _COMMON_INCLUDES_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef SIMULATION
#define SIMULATION  1
#endif

#if SIMULATION

    #include <stdint.h>

#else

    #include <types/vxTypes.h>

    #ifndef M_PI
    #define M_PI 3.14159265	
    #endif

#endif

#endif

