//*****************************************************************************
//
// $Created: JVT 02-12-18 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Random.inl $
// $Author: Nld $
// $Modtime: 7/10/04 15:55 $
// $Revision: 8 $
// $Workfile: MT_Random.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MT_Random::nextState
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
__forceinline
void MT_Random::nextState()
{
    assert( left_ == 0 );

    uint32 *p = state_;

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
__forceinline
uint32 MT_Random::rand32()
{
    if ( !--left_ )
        nextState();

    uint32 y = *next_++;

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
inline
double MT_Random::rand_ii()
{
    return (double)rand32() * ( 1. / 4294967295. ); /* divided by 2^32-1 */ 
}



//-----------------------------------------------------------------------------
// Name: MT_Random::rand_io
// generates a random number on [0,1)-real-interval
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand_io()
{
    return (double)rand32() * ( 1. / 4294967296. ); /* divided by 2^32 */
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oo
// generates a random number on (0,1)-real-interval
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand_oo()
{
    return ( (double)rand32() + 0.5 ) * ( 1. / 4294967296. ); /* divided by 2^32 */
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oi
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand_oi()
{
    return 1. - rand_io();
}



//-----------------------------------------------------------------------------
// Name: MT_Random::rand53
// generates a random number on [0,1) with 53-bit resolution
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand53()
{ 
    return ( ( rand32() >> 5 ) * 67108864. + ( rand32() >> 6 ) ) * ( 1. / 9007199254740992. ); 
} 


//-----------------------------------------------------------------------------
// Name: MT_Random::rand
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand_ii( double min, double max )
{
    return ( max - min ) * rand_ii() + min;
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand_io
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand_io( double min, double max )
{
    return ( max - min ) * rand_io() + min;
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oo
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand_oo( double min, double max )
{
    return ( max - min ) * rand_oo() + min;
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand_oi
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
double MT_Random::rand_oi( double min, double max )
{
    return ( max - min ) * rand_oi() + min;
}



//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_ii
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
uint32 MT_Random::rand32_ii( uint32 min, uint32 max )
{
    return (uint32)rand_io( min, max + 1 );
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_io
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
uint32 MT_Random::rand32_io( uint32 min, uint32 max )
{
    assert( max >= min );
    return (uint32)rand_io( min, max );
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_oo
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
uint32 MT_Random::rand32_oo( uint32 min, uint32 max )
{
    assert( max > ( min + 1 ) );
    return (uint32)rand_io( min + 1, max );
}


//-----------------------------------------------------------------------------
// Name: MT_Random::rand32_oi
// Created: JVT 03-10-15
//-----------------------------------------------------------------------------
inline
uint32 MT_Random::rand32_oi( uint32 min, uint32 max )
{
    assert( max >= ( min + 1 ) );
    return (uint32)rand_io( min + 1, max + 1 );
}

