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
 * @file mechanism.c
 */

/**
 * @addtogroup logger_mechanism
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include "logger-mechanism.h"
#include "_mechanism.h"
#include "_mechanism-syslog.h"
#include "_mechanism-stderr.h"
#include "_mechanism-null.h"



/** list of NftLogMechanism getters for various supported mechanisms */
static struct LogMechanisms
{
		NftLogMechanism *	(*get)(void);
}nft_log_mechanisms[] =
{
		{ &nft_log_mechanism_null },
		{ &nft_log_mechanism_stderr },
		{ &nft_log_mechanism_syslog },
		{ NULL }
};


/** currently used logging mechanism */
static NftLogMechanism *_current;




/**
 * get logging mechanism descriptor by name
 *
 * @param[in] name printable name of mechanism
 * @result NftLogMechanism or NULL
 */ 
static NftLogMechanism *_get(const char *name)
{
		if(!name)
				return NULL;
		
		/* walk through getters */
        for(struct LogMechanisms *mlist = nft_log_mechanisms; *mlist->get; mlist++)
		{
				NftLogMechanism *m = (*mlist->get)();
				if(strcmp(name, m->name) == 0)
				{
						return m;
				}
		}

		return NULL;
}


/**
 * log message using current mechanism
 *
 * @param[in] msg the message to log
 * @param[in] level the NftLoglevel of the message
 */
void _mechanism_log(NftLoglevel level, char *msg)
{
		/* get currently set logging mechanism name */
		char *mechanism_name;
		if(!(mechanism_name = getenv(NFT_LOG_ENV_MECHANISM)))
		{
				/* use default mechanism if none is set already */
				mechanism_name = NFT_LOG_DEFAULT_MECHANISM;
		}

		/* set current mechanism (will exit immediately if not necessary */
		if(!nft_log_mechanism_set(mechanism_name))
				return;
		
		/* log */
		if(_current->log)
				_current->log(level, msg);
}


/**
 * print a list of all available logging mechanisms to stdout
 */
void nft_log_mechanism_print_list()
{
		/* walk through getters */
        for(struct LogMechanisms *mlist = nft_log_mechanisms; *mlist->get; mlist++)
		{
				NftLogMechanism *m = (*mlist->get)();
				printf("%s ", m->name);
		}

		printf("\n");
}


/**
 * set current logging mechanism
 *
 * @param[in] name The valid name of a mechanism (s. @ref nft_log_mechanisms)
 * @result NFT_SUCCESS or NFT_FAILURE
 */
NftResult nft_log_mechanism_set(const char *name)
{
		if(!name)
		{
				fprintf(stderr, "No log mechanism name provided!\n");
				return NFT_FAILURE;
		}

		/* "list" mechanism to print a list of all mechanisms ? */
		if(strcmp(name, "list") == 0)
		{
				/* print list */
				printf("================================="
				       "=================================\n"
				       " available logging mechanisms:\n\t");
				nft_log_mechanism_print_list();
				printf("================================="
				       "=================================\n");

				/* use default mechanism */
				name = NFT_LOG_DEFAULT_MECHANISM;
		}
		
		/* same mechanism as before? */
		if(_current && strcmp(name, _current->name) == 0)
				return NFT_SUCCESS;
		
		/* deinitialize current mechanism */
		if(_current && _current->deinit)
		{
				_current->deinit();
				_current->initialized = false;
		}
		
		/* get new mechanism */
		if(!(_current = _get(name)))
		{
				fprintf(stderr, "Unknown logging mechanism: \"%s\"\n", name);
				return NFT_FAILURE;
		}

		/* initialize mechanism */
		if(_current->init)
		{
				if(!_current->init())
				{
						fprintf(stderr, "Failed to initialize mechanism \"%s\"\n", name);
						return NFT_FAILURE;
				}

				_current->initialized = true;
		}

		/* set environment variable */
        static char tmp[64];	
        snprintf(tmp, sizeof(tmp)-1, "%s=%s", NFT_LOG_ENV_MECHANISM, name);

        if(putenv(tmp) == -1)
                return NFT_FAILURE;
		
		return NFT_SUCCESS;
}

/**
 * @}
 */
