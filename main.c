/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * Program entry point.
 */

#include <avr/io.h>
#include <util/delay.h>

#include "state.h"

DatasetteState state;

int main(void)
{
  state_init(&state);

  /* Set up PORTB as output */
  DDRB = 0xFF;

  while (1)
  {
    /* Put the current state value out on PORTB. */
    PORTB = (unsigned char)state;
  }

  return 0;
}