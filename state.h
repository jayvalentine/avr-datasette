/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * Header file for state machine handling.
 */

/* Enum representing the state of the datasette.
 * Members:
 * IDLE           - no playback or recording.
 * PLAY           - playback of loaded tape image.
 * PLAY_PAUSED    - playback of loaded tape image paused by C64.
 * RECORD         - recording signal from C64.
 * RECORD_PAUSED  - recording of signal paused by C64.
 */

#ifndef STATE_H
#define STATE_H

enum datasette_state
{
  IDLE = 0,
  PLAY = 1,
  PLAY_PAUSED = 2,
  RECORD = 3,
  RECORD_PAUSED = 4
};

typedef enum datasette_state DatasetteState;

/* Function declarations. */

void state_init(DatasetteState* state);

#endif