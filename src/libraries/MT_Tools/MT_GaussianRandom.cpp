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

namespace
{
    double minRand = 1. / 4294967295.;
    double maxRand = 1. - minRand;
}

//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-04
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom()
    : rMean_           ( 0. )
    , rVariance_       ( 1. )
    , bAlreadyComputed_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-04
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom( double rMean, double rVariance )
    : rMean_           ( rMean )
    , rVariance_       ( rVariance )
    , bAlreadyComputed_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom constructor
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
MT_GaussianRandom::MT_GaussianRandom( const MT_GaussianRandom& rand )
    : rMean_           ( rand.rMean_ )
    , rVariance_       ( rand.rVariance_ )
    , bAlreadyComputed_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom copy operator
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
MT_GaussianRandom& MT_GaussianRandom::operator=( const MT_GaussianRandom& rhs )
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
    if( bAlreadyComputed_ )
    {
        bAlreadyComputed_ = false;
        return rAlreadyComputed_ + rMean_;
    }
    double rX         = MT_Random::GetInstance().rand_ii( -1., 1. );
    rAlreadyComputed_ = MT_Random::GetInstance().rand_ii( -1., 1. );
    double rSqLength  = rX * rX + rAlreadyComputed_ * rAlreadyComputed_;
    while ( rSqLength >= 1. || rSqLength == 0. )
    {
        rX                = MT_Random::GetInstance().rand_ii( -1., 1. );
        rAlreadyComputed_ = MT_Random::GetInstance().rand_ii( -1., 1. );
        rSqLength         = rX * rX + rAlreadyComputed_ * rAlreadyComputed_;
    }
    rSqLength          = rVariance_ * sqrt( ( -2. * log( rSqLength ) ) / rSqLength );
    bAlreadyComputed_  = true;
    rAlreadyComputed_ *= rSqLength;
    return rX * rSqLength + rMean_;
}

//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom::GetMean
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
double MT_GaussianRandom::GetMean() const
{
    return rMean_;
}

//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom::GetVariance
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
double MT_GaussianRandom::GetVariance() const
{
    return rVariance_;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_ii
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
double MT_GaussianRandom::gaussian_ii()
{
    if( rVariance_ == 0. )
        return rMean_;

    double r = rand();
    if( r < 0. )
        r = 0.;
    else if( r > 1. )
        r = 1.;
    return r;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_io
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
double MT_GaussianRandom::gaussian_io()
{
    if( rVariance_ == 0. )
    {
        if( rMean_ == 1. )
            return maxRand;
        return rMean_;
    }
    double r = rand();
    if( r < 0. )
        r = 0.;
    else if( r >= 1. )
        r = maxRand;
    return r;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_oo
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
double MT_GaussianRandom::gaussian_oo()
{
    if( rVariance_ == 0. )
    {
        if( rMean_ == 0. )
            return minRand;
        else if( rMean_ == 1. )
            return maxRand;
        return rMean_;
    }
    double r = rand();
    if( r <= 0. )
        r = minRand;
    else if( r >= 1. )
        r = maxRand;
    return r;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_oi
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
double MT_GaussianRandom::gaussian_oi()
{
    if( rVariance_ == 0 )
    {
        if( rMean_ == 0. )
            return minRand;
        return rMean_;
    }
    double r = rand();
    if( r <= 0. )
        r = minRand;
    else if( r > 1. )
        r = 1.;
    return r;
}
