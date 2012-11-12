/*
 * libniftylog - niftylight logging library
 * Copyright (C) 2006-2012 Daniel Hiepler <daniel@niftylight.de>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file logger.h
 */

/**
 * @defgroup logger Logging API
 * @brief Logging functionality.
 * - use @ref nft_log_level_set() to control which kind of log-messages are 
 *   supressed. You should do this initially to set the default @ref NftLoglevel.
 *   (@ref L_INFO or @ref L_ERROR would be a wise choice for example) 
 * - use @ref nft_log_level_get() to acquire the currently used @ref NftLoglevel
 * - use @ref NFT_LOG() to output printable strings to the user. \n
 * - use @ref nft_log_level_to_string() and nft_log_level_from_string() to 
 *   convert between @ref NftLoglevel and their printable names
 * @todo introduce pre-defined logging mechanisms to choose from (stderr, 
 * syslog, null,...)
 * @todo seperate nft_log_func_register() to feed logging into the library and
 * nft_log_mechanism_register() to use a custom output mechanism
 *     
 * Messages are printed to stderr as long as you don't register your own 
 * @ref NftLogFunc using @ref nft_log_func_register()
 * \n Other convenience macros include:
 * - \ref NFT_LOG_PERROR("foo") - output perror("foo") using the logging mechanism \n
 * - \ref NFT_LOG_NULL(-1) - print error about a received NULL pointer and return -1 \n
 * - \ref NFT_TODO() - use this to print some generic "todo" messages to mark an unimplemented feature
 * @{
 */

#ifndef _NFT_LOGGER_H
#define _NFT_LOGGER_H


#include <stdarg.h>
#include <errno.h>
#include <string.h>



/** name of environment variable to hold loglevel */
#define NFT_LOG_ENVNAME         "NFT_LOGLEVEL"

/** type for returning failure-status */
#ifndef NFT_RESULT_DEFINED
#define NFT_RESULT_DEFINED
typedef enum
{
        NFT_FAILURE = 0,
        NFT_SUCCESS
} NftResult;
#endif


/** available loglevels (used by @ref nft_log_level_set() and @ref NFT_LOG()) 
    (adjust also logger.c:_loglevel_names when adjusting this) */
typedef enum
{
        /** used to represent invalid loglevels */
        L_INVALID = -1,
        /** maximum-entry always at beginning of list */
        L_MAX,
        /** very very noisy output - don't send this in bug reports ;) */
        L_VERY_NOISY,
        /** jabberish or oftenly repeating notifications */
        L_NOISY,
        /** internal working notifications */
        L_DEBUG,
        /* rough internal working */
        L_VERBOSE,
        /** notification that's informative to the user but not vital */
        L_INFO,
        /** notfication the user should read */
        L_NOTICE,
        /** only print warnings & errors */
        L_WARNING,
        /** only important errors are printed */
        L_ERROR,
        /** no messages are printed */
        L_QUIET,
        /** LEAVE THIS AT THE END OF THE LIST */
        L_MIN
} NftLoglevel;

/** logging function that will be called for every log-message if registered with @ref nft_log_func_register() */
typedef void                    (NftLogFunc) (void *userdata, NftLoglevel level, const char *file, const char *func, int line, const char *msg);

/** convenience macro for nft_log() \n
 * @note No \\n is needed at end of string. \n
 * <b>Example:</b> NFT_LOG(LL_INFO, "Reading config file \"%s\"...", config); 
 */
#define NFT_LOG($level, $msg, ...) nft_log($level, __FILE__, __func__, __LINE__, $msg, ##__VA_ARGS__)
/** perror logging-functionality */
#define NFT_LOG_PERROR($msg) nft_log(L_ERROR, __FILE__, __func__, __LINE__, "%s: %s", $msg, strerror(errno))
/** NULL pointer error-msg & return abrevation */
#define NFT_LOG_NULL(ret) { nft_log(L_DEBUG, __FILE__, __func__, __LINE__, "NULL pointer received."); return ret; }


/* high-level TODO macro ;) */
#define NFT_TODO() NFT_LOG(L_ERROR, "Not implemented, yet. Please tell developers that you need this.")



void                            nft_log(NftLoglevel level, const char *file, const char *func, int line, const char *msg, ...);
void                            nft_log_va(NftLoglevel level, const char *file, const char *func, int line, const char *msg, va_list args);
void                            nft_log_func_register(NftLogFunc * func, void *userdata);
NftResult                       nft_log_level_set(NftLoglevel loglevel);
NftLoglevel                     nft_log_level_get();
const char                     *nft_log_level_to_string(NftLoglevel loglevel);
NftLoglevel                     nft_log_level_from_string(const char *name);

#endif /* _NFT_LOGGER_H */


/**
 * @}
 */
