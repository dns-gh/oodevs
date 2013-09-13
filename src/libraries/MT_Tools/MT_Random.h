//*****************************************************************************
//
// $Created: JVT 02-12-18 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Random.h $
// $Author: Nld $
// $Modtime: 7/10/04 15:53 $
// $Revision: 6 $
// $Workfile: MT_Random.h $
//
//*****************************************************************************

#ifndef __MT_Random_h_
#define __MT_Random_h_

#include <boost/noncopyable.hpp>

namespace MT_RandomConsts
{
    const unsigned int N = 624;
    const unsigned int M = 397;
}

//*****************************************************************************
// Created: JVT 02-12-18
// Last modified: JVT 03-10-15
//*****************************************************************************
class MT_Random : private boost::noncopyable
{
public:
    static MT_Random& GetInstance();
    static void Initialize( unsigned long seed );

public:
    virtual ~MT_Random() {}

    unsigned long rand32();                            // [ 0  , 0xffffffff ]
    unsigned long rand32_ii( unsigned long min, unsigned long max ); // [ min, max ]
    unsigned long rand32_io( unsigned long min, unsigned long max ); // [ min, max [
    unsigned long rand32_oo( unsigned long min, unsigned long max ); // ] min, max [
    unsigned long rand32_oi( unsigned long min, unsigned long max ); // ] min, max ]

    double rand_ii();                           // [ 0.  , 1.   ]
    double rand_io();                           // [ 0.  , 1.   [
    double rand_oo();                           // ] 0.  , 1.   [
    double rand_oi();                           // ] 0.  , 1.   ]
    double rand_ii( double min, double max );   // [ min., max. ]
    double rand_io( double min, double max );   // [ min., max. [
    double rand_oo( double min, double max );   // ] min., max. [
    double rand_oi( double min, double max );   // ] min., max. ]

private:
    MT_Random();
    void InitializeSeed( unsigned long seed );
    void NextState();

private:
    unsigned long  state_[ MT_RandomConsts::N ];
    unsigned long* next_;
    unsigned int   left_;
};

#endif // __MT_Random_h

//
// JVT : Encapsulation du générateur "Mersenne twister" :
//
/*
   A C-program for MT19937, with initialization improved 2002/2/10.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   This is a faster version by taking Shawn Cokus's optimization,
   Matthe Bellew's simplification, Isaku Wada's real version.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/
