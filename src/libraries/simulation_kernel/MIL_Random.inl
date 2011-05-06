// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand32
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
long MIL_Random::rand32()
{
    return MT_Random::GetInstance().rand32();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand32_ii
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
unsigned long MIL_Random::rand32_ii( unsigned long min, unsigned long max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return static_cast< unsigned long >( min + gaussian_ii( ctxt ) * ( max - min ) );
    else
        return MT_Random::GetInstance().rand32_ii( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand32_io
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
unsigned long MIL_Random::rand32_io( unsigned long min, unsigned long max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return unsigned long( min + gaussian_io( ctxt ) * ( max - min ) );
    else
        return MT_Random::GetInstance().rand32_io( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand32_oo
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
unsigned long MIL_Random::rand32_oo( unsigned long min, unsigned long max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return unsigned long( min + gaussian_oo( ctxt ) * ( max - min ) );
    else
        return MT_Random::GetInstance().rand32_oo( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand32_oi
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
unsigned long MIL_Random::rand32_oi( unsigned long min, unsigned long max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return unsigned long( min + gaussian_oi( ctxt ) * ( max - min ) );
    else
        return MT_Random::GetInstance().rand32_oi( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand53
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand53()
{
    return MT_Random::GetInstance().rand53();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_ii
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_ii( int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return gaussian_ii( ctxt );
    else
        return MT_Random::GetInstance().rand_ii();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_io
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_io( int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return gaussian_io( ctxt );
    else
        return MT_Random::GetInstance().rand_io();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_oo
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_oo( int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return gaussian_oo( ctxt );
    else
        return MT_Random::GetInstance().rand_oo();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_oi
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_oi( int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return gaussian_oi( ctxt );
    else
        return MT_Random::GetInstance().rand_oi();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_ii
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_ii( double min, double max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return min + gaussian_ii( ctxt ) * ( max - min );
    else
        return MT_Random::GetInstance().rand_ii( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_io
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_io( double min, double max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return min + gaussian_io( ctxt ) * ( max - min );
    else
        return MT_Random::GetInstance().rand_io( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_oo
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_oo( double min, double max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return min + gaussian_oo( ctxt ) * ( max - min );
    else
        return MT_Random::GetInstance().rand_oo( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_oi
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::rand_oi( double min, double max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return min + gaussian_oi( ctxt ) * ( max - min );
    else
        return MT_Random::GetInstance().rand_oi( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::random_shuffle
// Created: JSR 2011-05-05
// -----------------------------------------------------------------------------
template< typename T >
inline
void MIL_Random::random_shuffle( std::vector< T >& vector, int ctxt )
{
    boost::function< unsigned long( unsigned long ) > fun =  boost::bind( &MIL_Random::rand32_io, 0, _1, ctxt );
    std::random_shuffle( vector.begin(), vector.end(), fun );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_ii
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::gaussian_ii( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_ii();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_io
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::gaussian_io( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_io();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_oo
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::gaussian_oo( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_oo();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_oi
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
double MIL_Random::gaussian_oi( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_oi();
}
