/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * Header file for tap file handling.
 */

#ifndef TAP_H
#define TAP_H

#include "timer.h"
#include "c64.h"

/* The formula for calculating an interval from a data byte
 * in a TAP file is
 *
 * (8 * BYTE) / CYCLES
 *
 * Where CYCLES is the clock cycles per second of the PAL C64,
 * which is 985,248 (just under 1MHz).
 *
 * Therefore, with a single byte, the minimum cycle length is
 * (8 * 1) / 985,248 = 8,120 ns
 *
 * and the maximum cycle length is
 * (8 * 255) / 985,248 = 2,070,545 ns
 *
 * The smallest resolution of the ATMega 328's timer is dependent
 * on the clock speed, 16MHz.
 * Therefore, the smallest timer interval is 62.5 ns.
 *
 * Dividing our previous values by this interval gives us these
 * values for the minimum and maximum cycle lengths:
 *
 * Minimum: 130 cycles.
 * Maximum: 33129 cycles.
 *
 * As you can see, we can't measure this cycle time
 * using an 8-bit timer, but the ATMega's 16-bit timers
 * should do the trick!
 */

typedef unsigned short tap_interval;

/* Width of values in TAP file is 1 byte. */

typedef unsigned char tap_data_byte;

#endif