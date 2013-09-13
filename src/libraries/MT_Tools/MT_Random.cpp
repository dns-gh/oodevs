// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Random.h"

#include <cassert>
#include <ctime>

using namespace MT_RandomConsts;

namespace
{
    unsigned long twist( unsigned long u, unsigned long v )
    {
        return ( ( ( ( ( (u) & 0x80000000UL ) | ( (v) & 0x7fffffffUL ) ) ) >> 1 ) ^ ( (v) & 1UL ? 0x9908b0dfUL : 0UL ) );
    }
}

//-----------------------------------------------------------------------------
// Name: MT_Random constructor
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
void MT_Random::InitializeSeed( unsigned long seed )
{
    left_ = 1;
    next_ = 0;
    state_[0] = seed & 0xffffffffUL;
    for ( unsigned int j = 1; j < N; ++j )
    {
        state_[j] = ( 1812433253UL * ( state_[j - 1] ^ ( state_[j - 1] >> 30 ) ) + j );
        state_[j] &= 0xffffffffUL;  /* for >32 bit machines */
    }
}

//-----------------------------------------------------------------------------
// Name: MT_Random constructor
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
MT_Random::MT_Random()
{
    InitializeSeed( unsigned long( time( 0 ) ) );
}

// -----------------------------------------------------------------------------
// Name: MT_Random::Initialize
// Created: BAX 2013-09-16
// -----------------------------------------------------------------------------
void MT_Random::Initialize( unsigned long seed )
{
    GetInstance().InitializeSeed( seed );
}

// -----------------------------------------------------------------------------
// Name: MT_Random::GetInstance
// Created: JSR 2010-07-01
// -----------------------------------------------------------------------------
MT_Random& MT_Random::GetInstance()
{
    static MT_Random instance;
    return instance;
}

//-----------------------------------------------------------------------------
// Name: MT_Random::NextState
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
void MT_Random::NextState()
{
    assert( left_ == 0 );
    unsigned long *p = state_;
    left_ = MT_RandomConsts::N;
    next_ = state_;
    int j;
    for ( j = MT_RandomConsts::N - MT_RandomConsts::M + 1; --j; ++p )
        *p = *( p + MT_RandomConsts::M ) ^ twist( p[0], p[1] );
    for ( j = MT_RandomConsts::M; --j; ++p )
        *p = *( p + MT_RandomConsts::M - MT_RandomConsts::N ) ^ twist( p[0], p[1] );
    *p = *( p + MT_RandomConsts::M - MT_RandomConsts::N ) ^ twist( p[0], state_[0] );
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand32
// Generates a random number on [0,0xffffffff]-interval
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
unsigned long MT_Random::rand32()
{
    if ( !--left_ )
        NextState();
    unsigned long y = *next_++;
    y ^= ( y >> 11 );
    y ^= ( y << 7  ) & 0x9d2c5680UL;
    y ^= ( y << 15 ) & 0xefc60000UL;
    y ^= ( y >> 18 );
    return y;
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand_ii
// generates a random number on [0,1]-real-interval
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_ii()
{
    return static_cast< double >( rand32() ) * ( 1. / 4294967295. ); /* divided by 2^32-1 */
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand_io
// generates a random number on [0,1)-real-interval
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_io()
{
    return static_cast< double >( rand32() ) * ( 1. / 4294967296. ); /* divided by 2^32 */
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oo
// generates a random number on (0,1)-real-interval
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_oo()
{
    return ( static_cast< double >( rand32() ) + 0.5 ) * ( 1. / 4294967296. ); /* divided by 2^32 */
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oi
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_oi()
{
    return 1. - rand_io();
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_ii( double min, double max )
{
    return ( max - min ) * rand_ii() + min;
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand_io
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_io( double min, double max )
{
    return ( max - min ) * rand_io() + min;
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oo
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_oo( double min, double max )
{
    return ( max - min ) * rand_oo() + min;
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oi
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
double MT_Random::rand_oi( double min, double max )
{
    return ( max - min ) * rand_oi() + min;
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_ii
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
unsigned long MT_Random::rand32_ii( unsigned long min, unsigned long max )
{
    return static_cast< unsigned long >( rand_io( min, max + 1 ) );
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_io
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
unsigned long MT_Random::rand32_io( unsigned long min, unsigned long max )
{
    assert( max >= min );
    return static_cast< unsigned long >( rand_io( min, max ) );
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_oo
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
unsigned long MT_Random::rand32_oo( unsigned long min, unsigned long max )
{
    assert( max > ( min + 1 ) );
    return static_cast< unsigned long >( rand_io( min + 1, max ) );
}

//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_oi
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
unsigned long MT_Random::rand32_oi( unsigned long min, unsigned long max )
{
    assert( max >= ( min + 1 ) );
    return static_cast< unsigned long >( rand_io( min + 1, max + 1 ) );
}