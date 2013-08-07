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

/**
 * @file logger.c
 */

/**
 * @addtogroup logger
 * @{
 */

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "logger-mechanism.h"
#include "logger.h"
#include "config.h"
#include "_mechanism.h"



/** maximum length of log-message in bytes */
#define MAX_MSG_SIZE    4096


/** names of existing loglevels (must be synced with NftLoglevel definition!) */
static const char *_loglevel_names[] = {
        "verynoisy",
        "noisy",
        "debug",
        "verbose",
        "info",
        "notice",
        "warning",
        "error",
        "quiet"
};


/** 
 * holds a pointer to an external logging function that can be registered
 * using nft_log_func_register() 
 */
static NftLogFunc *_func;
/**
 * userdata for registered NftLogFunc
 */
static void *_uptr;
/**
 * current loglevel (fallback if ENV-Var isn't set)
 */
static NftLoglevel _level;





/**
 * va_list version of nft_log
 */
static void _log_va(NftLoglevel level,
                const char *file,
                const char *func, int line, const char *msg, va_list args)
{


        char *tmp;
        if(!(tmp = alloca(MAX_MSG_SIZE)))
        {
                perror("alloca");
                return;
        }

        /* print log-string */
        if(vsnprintf((char *) tmp, MAX_MSG_SIZE-1, msg, args) < 0)
        {
                fprintf(stderr, "Failed to print message: \"%s\"", msg);
                perror("vsnprintf");
                return;
        }

        /* if an external function is registered, pass everything through to it 
         */
        if(_func)
        {
                _func(_uptr, level, file, func, line, tmp);
        }       

		/* no critical message */
		if(level < L_WARNING)
		{
				_mechanism_log(level, tmp);
		}
		/* warning or error message, print loglevel */
		else
		{
				/* build message */
				char *message;
				if(!(message = alloca(MAX_MSG_SIZE)))
				{
						perror("alloca");
						return;
				}

				snprintf(message, MAX_MSG_SIZE-1, "%s: %s",
						nft_log_level_to_string(level), tmp);
				
				/* use current logging mechanism to print message */
				_mechanism_log(level, message);
		}
		
}


/**
 * va_list version of nft_log (more detailed version)
 */
static void _log_va_debug(NftLoglevel level,
                      const char *file,
                      const char *func,
                      int line, const char *msg, va_list args)
{


        char *tmp;
        if(!(tmp = alloca(MAX_MSG_SIZE)))
        {
                perror("alloca");
                return;
        }

        /* print log-string */
        if(vsnprintf(tmp, MAX_MSG_SIZE-1, msg, args) < 0)
        {
                perror("vsnprintf");
                return;
        }

        /* if an external function is registered, pass everything through to it 
         */
        if(_func)
        {
                _func(_uptr, level, file, func, line, tmp);
        }

		/* build message */
		char *message;
		if(!(message = alloca(MAX_MSG_SIZE)))
		{
				perror("alloca");
				return;
		}

		snprintf(message, MAX_MSG_SIZE-1, "%s:%d %s() %s: %s",
				file,
				line, func, nft_log_level_to_string(level), tmp);
		
		/* use current logging mechanism to print message */
		_mechanism_log(level, message);
}


/**
 * main logging function 
 * @note DON'T CALL FUNCTION DIRECTLY! - Use the NFT_LOG() macro instead!
 * @param[in] level @ref NftLoglevel this message should have
 * @param[in] file __FILE__
 * @param[in] func __FUNC__
 * @param[in] line __line__
 * @param[in] msg the log-message to output
 */
void nft_log(NftLoglevel level,
             const char *file,
             const char *func, int line, const char *msg, ...)
{
        NftLoglevel lcur = nft_log_level_get();
        if(lcur > level)
                return;



        /* build message */
        va_list ap;
        va_start(ap, msg);

        if(lcur <= L_DEBUG)
                _log_va_debug(level, file, func, line, msg, ap);
        else
                _log_va(level, file, func, line, msg, ap);

        va_end(ap);

}


/**
 * register an external logging function
 * @param[in] func a @ref NftLogFunc that should output a string to the user in some way
 * @param[in] userdata arbitrary pointer that will be passed to the NftLogFunc
 */
void nft_log_func_register(NftLogFunc * func, void *userdata)
{
        _func = func;
        _uptr = userdata;
}


/**
 * set current loglevel
 * @param[in] loglevel current @ref NftLoglevel
 * @result NFT_SUCCESS or NFT_FAILURE
 */
NftResult nft_log_level_set(NftLoglevel loglevel)
{
        /** @todo lock for threading */
        if(loglevel >= L_MIN || loglevel <= L_MAX)
                return NFT_FAILURE;

        /* set new loglevel */
        _level = loglevel;

        /* set new loglevel to environment variable */
        static char tmp[64];		
        snprintf(tmp, sizeof(tmp)-1, "%s=%s", NFT_LOG_ENV_LEVEL,
                 nft_log_level_to_string(loglevel));

        if(putenv(tmp) != 0)
		{
				perror("putenv");
                return NFT_FAILURE;
		}
		
        return NFT_SUCCESS;
}


/**
 * get current loglevel
 * @result the current @ref NftLoglevel
 */
NftLoglevel nft_log_level_get()
{
        /* valid environment variable set? */
        NftLoglevel l = nft_log_level_from_string(getenv(NFT_LOG_ENV_LEVEL));
        if(l >= L_MIN || l <= L_MAX)
        {
                return _level;
        }

        return l;
}


/**
 * return name of current loglevel
 *
 * @param[in] loglevel loglevel
 * @result string with loglevel name or NULL upon error
 */
const char *nft_log_level_to_string(NftLoglevel loglevel)
{
        if(loglevel >= L_MIN || loglevel <= L_MAX)
        {
                NFT_LOG(L_ERROR, "Invalid loglevel: %d", loglevel);
                return NULL;
        }

        return _loglevel_names[loglevel - 1];
}


/**
 * return loglevel of loglevel-name
 *
 * @param[in] name string with lowercase loglevel-name
 * @result loglevel or -1 upon error
 */
NftLoglevel nft_log_level_from_string(const char *name)
{
        if(!name)
                return L_INVALID;

        NftLoglevel res;

        for(res = L_MAX; res < L_MIN; res++)
        {
                if(strncmp
                   (name, _loglevel_names[res],
                    sizeof(_loglevel_names[res])) == 0)
                {
                        return res + 1;
                }
        }

        NFT_LOG(L_ERROR, "invalid loglevel name: \"%s\"", name);
        return L_INVALID;
}


/**
 * find out if loglevel a is more noisy than loglevel b
 *
 * @param[in] a NftLoglevel
 * @param[in] b NftLoglevel
 * @result true if a is more noisy than b, false if not or if loglevels are equal
 */
bool nft_log_level_is_noisier_than(NftLoglevel a, NftLoglevel b)
{
		if(a >= L_MIN || a <= L_MAX)
        {
                NFT_LOG(L_ERROR, "Invalid loglevel: %d", a);
                return false;
        }

		if(b >= L_MIN || b <= L_MAX)
        {
                NFT_LOG(L_ERROR, "Invalid loglevel: %d", b);
                return false;
        }
		   
		return a <= b ? true : false;
}


/**
 * print list of valid loglevel names to stdout
 *
 * @result print list of loglevels to stdout
 */
void nft_log_print_loglevels()
{
        NftLoglevel i;
        for(i = L_MAX + 1; i < L_MIN - 1; i++)
                printf("%s ", nft_log_level_to_string(i));
}


/**
 * @}
 */
