/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * Tap file handling.
 */

#include "tap.h"

tap_interval tap_byte_to_interval(tap_data_byte byte);

/* Given a doubly-indirected pointer to some TAP data,
 * increment the pointer so that it points to the next data in the file,
 * or 0 if EOF, and return the next interval.
 */

tap_interval tap_get_next_interval(tap_data_byte** data_ptr)
{
  /* Get the next interval. */
  tap_interval val = tap_byte_to_interval(**data_ptr);

  /* Increment the pointer. We aren't currently worrying about
   * V2 TAP files where intervals can be spread across multiple
   * data bytes...
   */
  *data_ptr++;

  return val;
}

/* Calculate a timer value from a data byte in a TAP file. */

tap_interval tap_byte_to_interval(tap_data_byte byte)
{
  /* Calculate the interval in nanoseconds. */
  unsigned int interval_ns = ((8 * (unsigned int)byte) / C64_CLOCK_SPEED);

  /* Divide by the timer interval to give the number of timer cycles. */
  tap_interval interval_cycles = interval_ns / TIMER_INTERVAL_NS;

  return interval_cycles;
}