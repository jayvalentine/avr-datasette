/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * Program entry point.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "state.h"

DatasetteState state;

int main(void)
{
  state_init(&state);

  /* Set up PORTB as output */
  DDRB = 0xFF;

  /* Set PORTC 0, 1, 2 as input */
  DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));

  /* Turn on pull-up resistors for PORTC 0, 1, 2 */
  PORTC |= ((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2));

  PCICR |= (1 << PCIE1);
  PCMSK1 |= ((1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10));

  sei();

  while (1)
  {
    /* Put the current state value out on PORTB. */
    PORTB = (unsigned char)state;
  }

  return 0;
}

ISR(PCINT1_vect)
{
  cli();
  unsigned char inputs = 0x00;

  /* High to low change on PC0 (PLAY) */
  if ((PINC & (1 << PINC0)) == 0)
  {
    inputs |= PLAY;
  }

  /* High to low change on PC1 (RECORD) */
  if ((PINC & (1 << PINC1)) == 0)
  {
    inputs |= RECORD;
  }

  /* High to low change on PC2 (STOP) */
  if ((PINC & (1 << PINC2)) == 0)
  {
    inputs |= STOP;
  }

  state_transition(&state, inputs);
  sei();
}