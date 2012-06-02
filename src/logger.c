/*
 * libniftylog - niftylight logging library
 * Copyright (C) 2006-2010 Daniel Hiepler <daniel@niftylight.de>
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
         
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "logger.h"
#include "config.h"

/** maximum length of log-message in bytes */
#define MAX_MSG_SIZE    4096


/** names of existing loglevels (must be synced with NftLoglevel definition!) */
static const char *_loglevel_names[] =
{
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
void nft_log_va(NftLoglevel level, const char * file, const char * func, int line, const char * msg, va_list args)
{


	char *tmp;
        if(!(tmp = alloca(MAX_MSG_SIZE)))
        {
                perror("alloca");
                return;
        }

	/* print log-string */
	if(vsnprintf((char *) tmp, MAX_MSG_SIZE, msg, args) < 0)
        {
                perror("vsnprintf");
                return;
        }
        
        /* if an external function is registered, pass everything through to it */
        if(_func)
        {
                _func(_uptr, level, file, func, line, tmp);
        }
        /* print to stderr */
        else
        {
                /* no critical message */
                if(level < L_WARNING)
                        fprintf(stderr, "%s\n", tmp);
                /* warning or error message, print loglevel */
                else
                        fprintf(stderr, "%s: %s\n", nft_log_level_to_string(level), tmp);
        }
}


/**
 * va_list version of nft_log (more detailed version)
 */
void nft_log_va_debug(NftLoglevel level, const char * file, const char * func, int line, const char * msg, va_list args)
{


	char *tmp;
        if(!(tmp = alloca(MAX_MSG_SIZE)))
        {
                perror("alloca");
                return;
        }

	/* print log-string */
	if(vsnprintf((char *) tmp, MAX_MSG_SIZE, msg, args) < 0)
        {
                perror("vsnprintf");
                return;
        }
        
        /* if an external function is registered, pass everything through to it */
        if(_func)
        {
                _func(_uptr, level, file, func, line, tmp);
        }
        /* print to stderr */
        else
        {
                fprintf(stderr, "%s:%d %s() %s: %s\n", 
				    file, 
				    line, 
				    func, 
				    nft_log_level_to_string(level), 
				    tmp);
        }
}


/**
 * main logging function - don't call directly - use NFT_LOG() instead
 * @param level @ref NftLoglevel this message should have
 * @param file __FILE__
 * @param func __FUNC__
 * @param line __line__
 * @param msg the log-message to output
 */
void nft_log(NftLoglevel level, const char * file, const char * func, int line, const char * msg, ...)
{
        NftLoglevel lcur = nft_log_level_get();
	if (lcur > level)
                return;

        
        
        /* build message */
        va_list ap;
        va_start(ap, msg);

	if(lcur <= L_DEBUG)
		nft_log_va_debug(level, file, func, line, msg, ap);
	else
		nft_log_va(level, file, func, line, msg, ap);
	
        va_end(ap);

}


/**
 * register an external logging function
 * @param func a @ref NftLogFunc that should output a string to the user in some way
 * @param userdata arbitrary pointer that will be passed to the NftLogFunc
 */
void nft_log_func_register(NftLogFunc *func, void *userdata)
{
    _func = func;
    _uptr = userdata;
}


/**
 * set loglevel
 * @param loglevel current @ref NftLoglevel
 * @result NFT_SUCCESS or NFT_FAILURE
 */
NftResult nft_log_level_set(NftLoglevel loglevel)
{
        /** @todo lock for threading */
        if(loglevel >= L_MIN || loglevel <= L_MAX)
                return NFT_FAILURE;

        _level = loglevel;
        
        static char tmp[64];
        snprintf(tmp, 64, "%s=%s", NFT_LOG_ENVNAME, nft_log_level_to_string(loglevel));
        
        if(putenv(tmp) == -1)
                return NFT_FAILURE;        

        return NFT_SUCCESS;
}


/**
 * get loglevel
 * @result the current @ref NftLoglevel or -1 upon error
 */
NftLoglevel nft_log_level_get()
{
        char *env;
        if(!(env = getenv(NFT_LOG_ENVNAME)))
        {
                return _level;
        }
        
        return nft_log_level_from_string(env);
}


/**
 * return name of current loglevel
 *
 * @p loglevel loglevel
 * @result string with loglevel name or NULL upon error
 */
const char *nft_log_level_to_string(NftLoglevel loglevel)
{
	if(loglevel >= L_MIN || loglevel <= L_MAX)
	{
		NFT_LOG(L_ERROR, "Invalid loglevel: %d", loglevel);
		return NULL;
	}

	return _loglevel_names[loglevel-1];
}


/**
 * return loglevel of loglevel-name
 *
 * @p name string with lowercase loglevel-name
 * @result loglevel or -1 upon error
 */
NftLoglevel nft_log_level_from_string(const char *name)
{
	if(!name)
		NFT_LOG_NULL(-1);
	
	NftLoglevel res;

	for(res=L_MAX; res < L_MIN; res++)
	{
		if(strncmp(name, _loglevel_names[res], sizeof(_loglevel_names[res])) == 0)
		{
			return res+1;
		}
	}

	NFT_LOG(L_ERROR, "invalid loglevel name: \"%s\"", name);
	return -1;
}


/**
 * @}
 */
