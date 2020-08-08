#ifndef ANIMATIONCOMMON_H
#define ANIMATIONCOMMON_H

#include "Common.h"

uint8_t beatquadwave8( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255,
                             uint32_t timebase = 0, uint8_t phase_offset = 0);

#endif
