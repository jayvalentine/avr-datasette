/* avr-datasette
 *
 * An AVR-based Commodore 64 datasette emulator.
 * Copyright (c) 2018 Jay Valentine.
 *
 * Header file for timer operation.
 */

#ifndef TIMER_H
#define TIMER_H

/* Minimum timer interval for the ATMega 328P
 * is 1/16MHz, or 62.5 ns. We don't want to deal with
 * nasty floating-point calculations, so we round this
 * to 63.
 */
#define TIMER_INTERVAL_NS 63UL

#endif