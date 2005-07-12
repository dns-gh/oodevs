//*****************************************************************************
//
// $Created: JVT 04-03-04 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_GaussianRandom.h $
// $Author: Nld $
// $Modtime: 1/09/04 16:43 $
// $Revision: 2 $
// $Workfile: MT_GaussianRandom.h $
//
//*****************************************************************************

#ifndef __MT_GaussianRandom_h_
#define __MT_GaussianRandom_h_

#include "MT_Random.h"

//*****************************************************************************
// Created: JVT 04-03-04
//*****************************************************************************
class MT_GaussianRandom
{
public:
    explicit MT_GaussianRandom();
    explicit MT_GaussianRandom( uint32 nSeed );
             MT_GaussianRandom( double rMean, double rVariance );
             MT_GaussianRandom( double rMean, double rVariance, uint32 nSeed );
    explicit MT_GaussianRandom( const MT_GaussianRandom& );

    MT_GaussianRandom& operator = ( const MT_GaussianRandom& );

    double rand();

    double GetMean()     const;
    double GetVariance() const;

private:
    double    rMean_;
    double    rVariance_;

    MT_Random linearRandom_;

    double    rAlreadyComputed_;
    bool      bAlreadyComputed_;
};

#include "MT_GaussianRandom.inl"


#endif // __MT_GaussianRandom_h_