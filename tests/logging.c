/*
 * libniftylog - niftylight logging library
 * Copyright (C) 2006-2013 Daniel Hiepler <daniel@niftylight.de>
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

#include <stdio.h>
#include <stdlib.h>
#include "niftylog.h"

#define MSG_VISIBLE		"This should be visible"
#define MSG_INVISIBLE	"This should *not* be visible"
#define MSG(l1,l2)		(nft_log_level_is_noisier_than(l1, l2) ? MSG_VISIBLE : MSG_INVISIBLE)
#define LOGWRAP(l1,l2)	NFT_LOG(l1, MSG(l2, l1))


/** set loglevel */
static bool _level_set(NftLoglevel l)
{
        /* set loglevel */
        if(!nft_log_level_set(l))
        {
                fprintf(stderr, "Failed to nft_log_level_set(\"%s\")!\n",
                        nft_log_level_to_string(l));
                return false;
        }

        /* check if loglevel has been set correctly */
        if(nft_log_level_get() != l)
        {
                fprintf(stderr, "Set loglevel != current loglevel!\n");
                return false;
        }

        return true;
}


/** set loglevel l and output message at all loglevels */
static bool _log_out(NftLoglevel l)
{
        if(!_level_set(l))
                return false;

        for(NftLoglevel a = L_MAX + 1; a < L_MIN; a++)
        {
                // printf("Current loglevel: %s - Message loglevel: %s\n",
                // nft_log_level_to_string(l), nft_log_level_to_string(a));
                LOGWRAP(a, l);
        }

        return true;
}


int main(int argc, char *argv[])
{
        NFT_LOG_CHECK_VERSION;

        /* clear loglevel environment variable */
        putenv(NFT_LOG_ENV_LEVEL);

        /* set all loglevels and output messages */
        for(NftLoglevel l = L_MAX + 1; l < L_MIN; l++)
                if(!_log_out(l))
                        return EXIT_FAILURE;

        return EXIT_SUCCESS;
}
