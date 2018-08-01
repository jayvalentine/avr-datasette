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
#include "tap.h"

DatasetteState state;

unsigned char timer_pulse_first_phase = 0;
tap_interval timer_next_interval = 0;

int main(void)
{
  /* Disable interrupts during set-up. */
  cli();

  state_init(&state);

  /* Set up PORTB as output */
  DDRB = 0xFF;

  /* Set up PORTD, pin 2 as input */
  DDRD &= ~(1 << DDD2);

  /* Set up PORTD, pin 3 as output, initially low. */
  DDRD |= (1 << DDD3);
  PORTD &= ~(1 << PORTD3);

  /* Pull-up for PD2 */
  PORTD |= (1 << PORTD2);

  /* Set PORTC 0, 1, 2 as input */
  DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));

  /* Turn on pull-up resistors for PORTC 0, 1, 2 */
  PORTC |= ((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2));

  /* Set up pin-change interrupts for PCINT8, PCINT9, PCINT10 */
  PCICR |= (1 << PCIE1);
  PCMSK1 |= ((1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10));

  /* Set up INT0, both rising- and falling-edge. */
  EICRA |= (1 << ISC00);
  EIMSK |= (1 << INT0);
  
  /* Initialize timer control register. */
  TCCR1A = 0;
  TCCR1B = 0;

  /* Enable timer compare interrupt. */
  TIMSK1 |= (1 << OCIE1A);

  /* Run timer at clock speed, 16MHz. */
  TCCR1B |= (1 << CS10);

  TCCR1B |= (1 << WGM12);

  /* Set initial value of timer to 0xFFFF,
   * so that it will trigger eventually once
   * we enable interrupts.
   */
  OCR1A = 0xFFFF;

  /* Enable interrupts. */
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

/* Interrupt handler for INT0.
 * Triggers when the state of MOTOR changes.
 */
ISR(INT0_vect)
{
  cli();

  /* Quick and dirty debouncing */
  _delay_ms(10);

  unsigned char inputs = 0x00;

  /* If motor is now high, we are on a rising edge.
   * Otherwise, we are on a falling edge.
   *
   * Because of how the 4n25 optocoupler is wired,
   * Pin 2 will be LOW when the motor control is HIGH,
   * and will be HIGH when motor control is LOW.
   *
   * Therefore:
   * - Rising edge: Motor has just switched OFF.
   * - Falling edge: Motor has just switched ON.
   */
  if (PIND & (1 << PIND2))
  {
    inputs |= MOTOR_OFF;
  }
  else if ((PIND & (1 << PIND2)) == 0)
  {
    inputs |= MOTOR_ON;
  }

  state_transition(&state, inputs);

  sei();
}

ISR(TIMER1_COMPA_vect)
{
  cli();

  /* If we have just finished the first half of a cycle,
   * set output LOW.
   *
   * Otherwise, get the next data byte, set timer accordingly,
   * and set output HIGH.
   */
  if (timer_pulse_first_phase)
  {
    timer_pulse_first_phase = 0;

    PORTD ^= (1 << PORTD3);
  }
  else
  {
    timer_pulse_first_phase = 1;

    PORTD ^= (1 << PORTD3);

    /* Hardcoded test value for now. */
    timer_next_interval = 33129 / 2;
  }

  /* Set timer interval again. */
  OCR1A = timer_next_interval;

  sei();
}