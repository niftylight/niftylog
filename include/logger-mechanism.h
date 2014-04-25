/*
 * libniftylog - niftylight logging library
 * Copyright (C) 2006-2014 Daniel Hiepler <daniel@niftylight.de>
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
 * @file logger-mechanism.h
 */

/**
 * @addtogroup logger
 * @{ 
 * @defgroup logger_mechanism NftLogMechanism
 * @brief API to provide different logging mechanisms
 * 
 * To choose a logging mechanism, either set the NFT_LOG_MECHANISM environment
 * variable or use @ref nft_log_mechanism_set() from within the application.
 *
 * A list of available logging mechanisms is printed on stdout either by calling
 * @ref nft_log_mechanism_print_list() or by setting NFT_LOG_MECHANISM="list"
 *
 * Each logging mechanism may use own NFT_LOG_... environment variables for
 * configuration.
 * 
 * To add a new logging mechanism:
 * - add mechanism-foo.c and _mechanism-foo.h to /src directory
 *   (and add to src/Makefile.am) 
 * - add the @ref NftLogMechanism descriptor getter function to the
 *   mechanism.c:nft_log_mechanisms structure
 * - implement all needed functions described by the @ref NftLogMechanism descriptor
 *   (any function can be NULL if it's not needed)
 * @{
 */

#ifndef _NFT_LOG_MECHANISM_H
#define _NFT_LOG_MECHANISM_H

#include "logger.h"


/** default logging mechanism */
#define NFT_LOG_DEFAULT_MECHANISM	"stderr"


/** logging mechanism descriptor */
typedef struct
{
        /** name of this mechanism */
        const char                      name[64];
        /** logging function of this mechanism */
        void                            (*log) (NftLoglevel level, const char *msg);
        /** initialization function of this mechanism */
        NftResult                       (*init) (void);
        /** deinitialization function of this mechanism */
        void                            (*deinit) (void);
        /** set to true if mechanism is initialized */
        bool                            initialized;
} NftLogMechanism;







void                            nft_log_mechanism_print_list();
NftResult                       nft_log_mechanism_set(const char *name);


#endif /* _NFT_LOG_MECHANISM_H */


/**
 * @}
 * @}
 */
