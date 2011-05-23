// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Random.h"
#include <ctime>

using namespace MT_RandomConsts;

MT_Random* MT_Random::pInstance_ = 0;

//-----------------------------------------------------------------------------
// Name: MT_Random constructor
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
MT_Random::MT_Random( unsigned long nSeed )
    : left_ ( 1 )
{
    state_[0] = nSeed & 0xffffffffUL;
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
    : left_ ( 1 )
{
    this->MT_Random::MT_Random( unsigned long( time( 0 ) ) );
}

//-----------------------------------------------------------------------------
// Name: MT_Random constructor
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
MT_Random::MT_Random( const std::vector< unsigned long >& init_key )
    : left_ ( 1 )
{
    this->MT_Random::MT_Random( 19650218UL );
    unsigned int i = 1;
    unsigned int j = 0;
    unsigned int k = __max(  N, static_cast< unsigned >( init_key.size() ) );
    for (; k; --k)
    {
        state_[i] = ( state_[i] ^ ( ( state_[i - 1] ^ ( state_[i - 1] >> 30 ) ) * 1664525UL ) ) + init_key[j] + j;
        state_[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        ++i; ++j;
        if( i >= N )
        {
            state_[0] = state_[N - 1];
            i = 1;
        }
        if (j >= init_key.size() )
            j = 0;
    }
    for ( k = N - 1; k; --k)
    {
        state_[i] = ( state_[i] ^ ( ( state_[i - 1] ^ ( state_[i - 1] >> 30 ) ) * 1566083941UL ) ) - i;
        state_[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        ++i;
        if( i >= N )
        {
            state_[0] = state_[N - 1];
            i = 1;
        }
    }
    state_[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}
