#include "AnimationCommon.h"

uint8_t beatquadwave8( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255,
                             uint32_t timebase = 0, uint8_t phase_offset = 0)
{
  uint8_t beat = beat8( beats_per_minute, timebase);
  uint8_t beatsin = quadwave8( beat + phase_offset);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8( beatsin, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}
