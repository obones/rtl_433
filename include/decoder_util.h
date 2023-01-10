/** @file
    High-level utility functions for decoders.

    Copyright (C) 2018 Christian Zuckschwerdt

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/

#ifndef INCLUDE_DECODER_UTIL_H_
#define INCLUDE_DECODER_UTIL_H_

#include <stdarg.h>
#include "bitbuffer.h"
#include "data.h"
#include "r_device.h"

#if defined _MSC_VER || defined ESP32 // Microsoft Visual Studio or ESP32
    // MSC and ESP32 have something like C99 restrict as __restrict
    #ifndef restrict
    #define restrict  __restrict
    #endif
#endif
// Defined in newer <sal.h> for MSVC.
#ifndef _Printf_format_string_
#define _Printf_format_string_
#endif

/// Create a new r_device, copy from dev_template if not NULL.
r_device *create_device(r_device const *dev_template);

/// Output data.
void decoder_output_data(r_device *decoder, data_t *data);

/// Output log.
void decoder_output_log(r_device *decoder, int level, data_t *data);

#ifndef RTL_433_DISABLE_DECODER_LOG
// be terse, a maximum msg length of 60 characters is supported on the decoder_log_ functions
// e.g. "FoobarCorp-XY3000: unexpected type code %02x"

/// Output a log message.
void decoder_log(r_device *decoder, int level, char const *func, char const *msg);

/// Output a formatted log message.
void decoder_logf(r_device *decoder, int level, char const *func, _Printf_format_string_ const char *format, ...)
#if defined(__GNUC__) || defined(__clang__)
        __attribute__((format(printf, 4, 5)))
#endif
        ;

/// Output a log message with the content of the bitbuffer.
void decoder_log_bitbuffer(r_device *decoder, int level, char const *func, const bitbuffer_t *bitbuffer, char const *msg);

/// Output a formatted log message with the content of the bitbuffer.
void decoder_logf_bitbuffer(r_device *decoder, int level, char const *func, const bitbuffer_t *bitbuffer, _Printf_format_string_ const char *format, ...)
#if defined(__GNUC__) || defined(__clang__)
        __attribute__((format(printf, 5, 6)))
#endif
        ;

/// Output a log message with the content of a bit row (byte buffer).
void decoder_log_bitrow(r_device *decoder, int level, char const *func, uint8_t const *bitrow, unsigned bit_len, char const *msg);

/// Output a formatted log message with the content of a bit row (byte buffer).
void decoder_logf_bitrow(r_device *decoder, int level, char const *func, uint8_t const *bitrow, unsigned bit_len, _Printf_format_string_ const char *format, ...)
#if defined(__GNUC__) || defined(__clang__)
        __attribute__((format(printf, 6, 7)))
#endif
        ;
#else
// use "empty macros" so that the unused arguments given hard coded strings will not appear in the final binary
// Note that te decoder_logf method uses its variadic arguments to avoid emitting warnings about unused variables in some decoders
// calling printf with an empty format string to get a noop. But as this emits a warning, we disable it.
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wformat-zero-length"
#endif
#define decoder_log(decoder, level, func, msg) /**/
#define decoder_logf(decoder, level, func, format, ...) printf("", ## __VA_ARGS__) /**/
#define decoder_log_bitbuffer(decoder, level, func, bitbuffer, msg) /**/
#define decoder_logf_bitbuffer(decoder, level, func, bitbuffer, format, ...) /**/
#define decoder_log_bitrow(decoder, level, func, bitrow, bit_len, msg) /**/
#define decoder_logf_bitrow(decoder, level, func, bitrow, bit_len, format, ...) /**/
#endif

#endif /* INCLUDE_DECODER_UTIL_H_ */
