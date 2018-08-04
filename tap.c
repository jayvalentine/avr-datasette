/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * Tap file handling.
 */

#include "tap.h"

#include <stdio.h>
#include "uart.h"

tap_interval tap_byte_to_interval(tap_data_byte byte);

/* Given a doubly-indirected pointer to some TAP data,
 * increment the pointer so that it points to the next data in the file,
 * or 0 if EOF, and return the next interval.
 */

tap_interval tap_get_next_interval(tap_data_byte** data_ptr, tap_data_byte* end)
{
  /* Get the next interval. */
  tap_interval val = tap_byte_to_interval(**data_ptr);

  /* Increment the pointer. We aren't currently worrying about
   * V2 TAP files where intervals can be spread across multiple
   * data bytes...
   */
  *data_ptr++;

  /* If the pointer is now beyond the end address,
   * set to 0.
   */
  if (*data_ptr > end)
  {
    *data_ptr = 0;
  }

  return val;
}

/* Calculate a timer value from a data byte in a TAP file. */

tap_interval tap_byte_to_interval(tap_data_byte byte)
{
  /* Calculate the interval in nanoseconds. */
  unsigned long long interval_ns = (((unsigned long long)8 * ((unsigned long long)byte * NANOSECOND_SCALING)) / C64_CLOCK_SPEED);

  /* Divide by the timer interval to give the number of timer cycles. */
  tap_interval interval_cycles = (tap_interval)(interval_ns / TIMER_INTERVAL_NS);

  return interval_cycles;
}