//*****************************************************************************
//
// $Created: JVT 04-03-04 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_GaussianRandom.cpp $
// $Author: Nld $
// $Modtime: 1/09/04 16:43 $
// $Revision: 2 $
// $Workfile: MT_GaussianRandom.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_GaussianRandom.h"

//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-04
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom()
    : rMean_            ( 0. )
    , rVariance_        ( 1. )
    , linearRandom_     ()
    , bAlreadyComputed_ ( false )
{
    
}


//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-04
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom( uint32 nSeed )
    : rMean_            ( 0. )
    , rVariance_        ( 1. )
    , linearRandom_     ( nSeed )
    , bAlreadyComputed_ ( false )
{
}



//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-04
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom( double rMean, double rVariance )
    : rMean_            ( rMean )
    , rVariance_        ( rVariance )
    , linearRandom_     ()
    , bAlreadyComputed_ ( false )
{
}


//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-04
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom( double rMean, double rVariance, uint32 nSeed )
    : rMean_            ( rMean )
    , rVariance_        ( rVariance )
    , linearRandom_     ( nSeed )
    , bAlreadyComputed_ ( false )
{
}



//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom( const MT_GaussianRandom& rand)
    : rMean_            ( rand.rMean_ )
    , rVariance_        ( rand.rVariance_ )
    , linearRandom_     ()
    , bAlreadyComputed_ ( false )
{
}



//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom copy operator
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
MT_GaussianRandom& MT_GaussianRandom::operator = ( const MT_GaussianRandom& rhs )
{
    rMean_            = rhs.rMean_;
    rVariance_        = rhs.rVariance_;
    bAlreadyComputed_ = false;

    return *this;
}



//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom::rand
// Created: JVT 04-03-04
//-----------------------------------------------------------------------------
// Transformation de Box-Muller sous forme polaire
double MT_GaussianRandom::rand()
{
    if ( bAlreadyComputed_ )
    {
        bAlreadyComputed_ = false;
        return rAlreadyComputed_ + rMean_;
    }

    double rX         = linearRandom_.rand_ii( -1., 1. );
    rAlreadyComputed_ = linearRandom_.rand_ii( -1., 1. );
    double rSqLength  = rX * rX + rAlreadyComputed_ * rAlreadyComputed_;

    while ( rSqLength >= 1. || rSqLength == 0. )
    {
        rX                = linearRandom_.rand_ii( -1., 1. );
        rAlreadyComputed_ = linearRandom_.rand_ii( -1., 1. );
        rSqLength         = rX * rX + rAlreadyComputed_ * rAlreadyComputed_;
    }

    rSqLength          = rVariance_ * sqrt( ( -2. * log( rSqLength ) ) / rSqLength );
    bAlreadyComputed_  = true;
    rAlreadyComputed_ *= rSqLength;

    return rX * rSqLength + rMean_;
}
