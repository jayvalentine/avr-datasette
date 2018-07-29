/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * State machine handling.
 */

#include "state.h"

void state_init(DatasetteState* state)
{
  *state = IDLE;
}