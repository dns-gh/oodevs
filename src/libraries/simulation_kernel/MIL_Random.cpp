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

MIL_Random* MIL_Random::pInstance_ = 0;
std::vector< MT_GaussianRandom* > MIL_Random::gaussianRandom_;

// -----------------------------------------------------------------------------
// Name: MIL_Random::Initialize
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
void MIL_Random::Initialize( int nSeed, const bool* bGaussian, const int* nDeviation, const int* nMean )
{
    if( pInstance_ )
        throw std::runtime_error( "Random Generator already initialized" );

    pInstance_ = new MIL_Random( nSeed, bGaussian, nDeviation, nMean );
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
MIL_Random::MIL_Random( int nSeed, const bool* bGaussian, const int* nDeviation, const int* nMean )
{
    if( nSeed )
        MT_Random::Initialize( nSeed );

    for( int i = 0; i < eContextsNbr; ++i )
        gaussianRandom_.push_back( bGaussian[ i ] ? new MT_GaussianRandom( 0.1 * nMean[ i ], 0.01 * nDeviation[ i ]* nDeviation[ i ] ) : 0 ); // variance = ecart-type², nDeviation entre 0 et 10
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
