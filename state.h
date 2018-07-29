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
  IDLE = 0x00,
  PLAY = 0x01,
  PLAY_PAUSED = 0x05,
  RECORD = 0x02,
  RECORD_PAUSED = 0x06
};

typedef enum datasette_state DatasetteState;

#define PLAY (1)
#define RECORD (1 << 1)
#define STOP (1 << 2)
#define MOTOR (1 << 3)

/* Function declarations. */

void state_init(DatasetteState* state);

void state_transition(DatasetteState* state, unsigned char inputs);

#endif