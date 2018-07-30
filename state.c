/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * State machine handling.
 */

#include "state.h"

/* Private function declarations. */

void state_transition_idle(DatasetteState* state, unsigned char inputs);

void state_transition_play(DatasetteState* state, unsigned char inputs);

void state_transition_record(DatasetteState* state, unsigned char inputs);

void state_transition_play_paused(DatasetteState* state, unsigned char inputs);

void state_transition_record_paused(DatasetteState* state, unsigned char inputs);

/* Function definitions. */

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
  else if (*state == PLAY)
  {
    state_transition_play(state, inputs);
  }
  else if (*state == RECORD)
  {
    state_transition_record(state, inputs);
  }
  else if (*state == PLAY_PAUSED)
  {
    state_transition_play_paused(state, inputs);
  }
  else if (*state ==  RECORD_PAUSED)
  {
    state_transition_record_paused(state, inputs);
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

void state_transition_play(DatasetteState* state, unsigned char inputs)
{
  if (inputs & STOP)
  {
    *state = IDLE;
  }
  else if (inputs & MOTOR_OFF)
  {
    *state = PLAY_PAUSED;
  }
}

void state_transition_record(DatasetteState* state, unsigned char inputs)
{
  if (inputs & STOP)
  {
    *state = IDLE;
  }
  else if (inputs & MOTOR_OFF)
  {
    *state = RECORD_PAUSED;
  }
}

void state_transition_play_paused(DatasetteState *state, unsigned char inputs)
{
  if (inputs & STOP)
  {
    *state = IDLE;
  }
  else if (inputs & MOTOR_ON)
  {
    *state = PLAY;
  }
}

void state_transition_record_paused(DatasetteState *state, unsigned char inputs)
{
  if (inputs & STOP)
  {
    *state = IDLE;
  }
  if (inputs & MOTOR_ON)
  {
    *state = RECORD;
  }
}