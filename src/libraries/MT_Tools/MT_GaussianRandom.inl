//*****************************************************************************
//
// $Created: JVT 04-03-04 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_GaussianRandom.inl $
// $Author: Nld $
// $Modtime: 1/09/04 16:43 $
// $Revision: 2 $
// $Workfile: MT_GaussianRandom.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom::GetMean
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
inline
double MT_GaussianRandom::GetMean() const
{
    return rMean_;
}


//-----------------------------------------------------------------------------
// Name: MT_GaussianRandom::GetVariance
// Created: JVT 04-03-08
//-----------------------------------------------------------------------------
inline
double MT_GaussianRandom::GetVariance() const
{
    return rVariance_;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_ii
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
inline
double MT_GaussianRandom::gaussian_ii()
{
    if( rVariance_ == 0. && rMean_ == 0. )
        return 0.;

    if( rVariance_ == 0. && rMean_ == 1. )
        return 1.;

    double r = rand();
    while( r < 0. || r > 1. )
        r = rand();
    return r;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_io
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
inline
double MT_GaussianRandom::gaussian_io()
{
    if( rVariance_ == 0. && rMean_ == 0. )
        return 0.;

    if( rVariance_ == 0. && rMean_ == 1. )
        return 1. - ( 1. / 4294967295. );

    double r = rand();
    while( r < 0. || r >= 1. )
        r = rand();
    return r;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_oo
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
inline
double MT_GaussianRandom::gaussian_oo()
{
    if( rVariance_ == 0. && rMean_ == 0. )
        return 1. / 4294967295.;

    if( rVariance_ == 0. && rMean_ == 1. )
        return 1. - ( 1. / 4294967295. );

    double r = rand();
    while( r <= 0. || r >= 1. )
        r = rand();
    return r;
}

// -----------------------------------------------------------------------------
// Name: MT_GaussianRandom::gaussian_oi
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
inline
double MT_GaussianRandom::gaussian_oi()
{
    if( rVariance_ == 0. && rMean_ == 0. )
        return 1. / 4294967295.;

    if( rVariance_ == 0. && rMean_ == 1. )
        return 1.;

    double r = rand();
    while( r <= 0. || r > 1. )
        r = rand();
    return r;
}
