/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * State machine handling.
 */

#include "state.h"

void state_transition_idle(DatasetteState* state, unsigned char inputs);

void state_init(DatasetteState* state)
{
  *state = IDLE;
}

void state_transition(DatasetteState* state, unsigned char inputs)
{
  /* Inputs is a byte where the lower 4 bits represent one of 4
   * inputs.
   * Bit 0: Play
   * Bit 1: Record
   * Bit 2: Stop
   * BIt 3: Motor
   */
  if (*state == IDLE)
  {
    state_transition_idle(state, inputs);
  }
}

void state_transition_idle(DatasetteState* state, unsigned char inputs)
{
  if (inputs & PLAY)
  {
    *state = PLAY;
  }
  else if (inputs & RECORD)
  {
    *state = RECORD;
  }
}