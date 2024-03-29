/*
   Copyright (c) 2012, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#include <my_rnd.h>

#if defined(HAVE_YASSL)

#if defined(YASSL_PREFIX)
#define RAND_bytes yaRAND_bytes
#endif /* YASSL_PREFIX */

#include <openssl/ssl.h>

#elif defined(HAVE_OPENSSL)
#include <openssl/rand.h>
#endif /* HAVE_YASSL */


/*
  A wrapper to use OpenSSL/yaSSL PRNGs.
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
  Generate a random number using the OpenSSL/yaSSL supplied
  random number generator if available.

  @param rand_st [INOUT] Structure used for number generation
                         only if none of the SSL libraries are
                         available.

  @retval                Generated random number.
*/

double my_rnd_ssl(struct rand_struct *rand_st)
{

#if defined(HAVE_YASSL) || defined(HAVE_OPENSSL)
  int rc;
  unsigned int res;

#if defined(HAVE_YASSL)
  rc= yaSSL::RAND_bytes((unsigned char *) &res, sizeof (unsigned int));
#else
  rc= RAND_bytes((unsigned char *) &res, sizeof (unsigned int));
#endif /* HAVE_YASSL */

  if (rc)
    return (double)res / (double)UINT_MAX;
  else
#endif /* defined(HAVE_YASSL) || defined(HAVE_OPENSSL) */
    return my_rnd(rand_st);
}

#ifdef __cplusplus
}
#endif
