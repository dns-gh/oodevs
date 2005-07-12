//*****************************************************************************
//
// $Created: JVT 02-12-18 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Random.cpp $
// $Author: Nld $
// $Modtime: 7/10/04 15:53 $
// $Revision: 5 $
// $Workfile: MT_Random.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Random.h"

#include <ctime>

using namespace MT_RandomConsts;

//-----------------------------------------------------------------------------
// Name: MT_Random constructor
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
MT_Random::MT_Random( uint32 nSeed )
    : left_ ( 1 )
{
    state_[0] = nSeed & 0xffffffffUL;
    for ( uint j = 1; j < N; ++j )
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
    this->MT_Random::MT_Random( time(0) );
}



//-----------------------------------------------------------------------------
// Name: MT_Random constructor
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
MT_Random::MT_Random( const std::vector< unsigned long >& init_key )
    : left_ ( 1 )
{
    this->MT_Random::MT_Random( 19650218UL );

    uint i = 1;
    uint j = 0;
    uint k = __max(  N, init_key.size() );
    
    for (; k; --k) 
    {
        state_[i] = ( state_[i] ^ ( ( state_[i - 1] ^ ( state_[i - 1] >> 30 ) ) * 1664525UL ) ) + init_key[j] + j;
        state_[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        ++i; ++j;
        if ( i >= N )
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
        if ( i >= N ) 
        { 
            state_[0] = state_[N - 1]; 
            i = 1;
        }
    }

    state_[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}
