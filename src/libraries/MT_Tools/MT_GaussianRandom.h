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
             MT_GaussianRandom();
             MT_GaussianRandom( double rMean, double rVariance );
    explicit MT_GaussianRandom( const MT_GaussianRandom& );
    virtual ~MT_GaussianRandom() {}

    MT_GaussianRandom& operator = ( const MT_GaussianRandom& );

    double rand();

    double gaussian_ii();
    double gaussian_io();
    double gaussian_oo();
    double gaussian_oi();

    double GetMean()     const;
    double GetVariance() const;

private:
    double    rMean_;
    double    rVariance_;

    double    rAlreadyComputed_;
    bool      bAlreadyComputed_;
};

#endif // __MT_GaussianRandom_h_
