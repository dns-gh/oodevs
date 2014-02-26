// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Random.h"
#include "MT_Tools/MT_Random.h"
#include "MT_Tools/MT_GaussianRandom.h"

MIL_Random* MIL_Random::pInstance_ = 0;
std::vector< MT_GaussianRandom* > MIL_Random::gaussianRandom_;

// -----------------------------------------------------------------------------
// Name: MIL_Random::Initialize
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
void MIL_Random::Initialize( int nSeed, const std::vector< bool >& bGaussian,
    const std::vector< double >& rDeviation, const std::vector< double >& rMean )
{
    if( pInstance_ )
        throw MASA_EXCEPTION( "Random Generator already initialized" );
    pInstance_ = new MIL_Random( nSeed, bGaussian, rDeviation, rMean );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::Terminate
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
void MIL_Random::Terminate()
{
    delete pInstance_;
    pInstance_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Random constructor
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
MIL_Random::MIL_Random( int nSeed, const std::vector< bool >& bGaussian,
     const std::vector< double >& rDeviation, const std::vector< double >& rMean )
{
    if( nSeed )
        MT_Random::Initialize( nSeed );
    for( int i = 0; i < eContextsNbr; ++i )
        gaussianRandom_.push_back( bGaussian.at( i  )
            ? new MT_GaussianRandom( rMean.at( i ), rDeviation.at( i )*rDeviation.at( i ) )
            : 0 ); // variance = ecart-type^2
}

// -----------------------------------------------------------------------------
// Name: MIL_Random destructor
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
MIL_Random::~MIL_Random()
{
    for( unsigned int i = 0; i < gaussianRandom_.size(); ++i )
        delete gaussianRandom_[ i ];
    gaussianRandom_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand32
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
long MIL_Random::rand32()
{
    return MT_Random::GetInstance().rand32();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand32_ii
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
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
unsigned long MIL_Random::rand32_oi( unsigned long min, unsigned long max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return unsigned long( min + gaussian_oi( ctxt ) * ( max - min ) );
    else
        return MT_Random::GetInstance().rand32_oi( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::rand_ii
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
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
double MIL_Random::rand_oi( double min, double max, int ctxt /* = -1 */ )
{
    if( ctxt != -1 && gaussianRandom_[ ctxt ] )
        return min + gaussian_oi( ctxt ) * ( max - min );
    else
        return MT_Random::GetInstance().rand_oi( min, max );
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_ii
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
double MIL_Random::gaussian_ii( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_ii();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_io
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
double MIL_Random::gaussian_io( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_io();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_oo
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
double MIL_Random::gaussian_oo( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_oo();
}

// -----------------------------------------------------------------------------
// Name: MIL_Random::gaussian_oi
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
double MIL_Random::gaussian_oi( int ctxt )
{
    return gaussianRandom_[ ctxt ]->gaussian_oi();
}
