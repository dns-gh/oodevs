// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Ellipse.h"
#include <cassert>

//-----------------------------------------------------------------------------
// Name: MT_Ellipse constructor
// Created: JVT 04-02-26
// Last modified: JVT 04-03-25
//-----------------------------------------------------------------------------
MT_Ellipse::MT_Ellipse( const MT_Vector2D& vOrigin, double rMajorAxeLength, double rMinorAxeLength, double rTheta )
    : rMajorAxeLength_( rMajorAxeLength )
    , rMinorAxeLength_( rMinorAxeLength )
    , rTheta_         ( rTheta )
    , rA_             ( rMajorAxeLength_ * rMajorAxeLength_ )
    , rB_             ( rA_ )
    , rC_             ( rMinorAxeLength_ * rMinorAxeLength_ )
    , rF_             ( -rA_ * rC_ )
    , vCenter_        ( vOrigin )
{
    if( rMajorAxeLength_ < rMinorAxeLength_ )
    {
        std::swap( rMajorAxeLength_, rMinorAxeLength_ );
        std::swap( rA_, rC_ );
        rB_ = rA_;
        rTheta_ += MT_PI / 2.;
    }

    assert( rA_ >= rC_ );

    double xc = sqrt( rA_ - rC_ );
    double yc = sin( rTheta_ ) * xc;
    xc *= cos( rTheta_ );

    rA_ -= xc * xc;
    rB_ -= yc * yc;
    rC_ = -2. * xc * yc;
}



//-----------------------------------------------------------------------------
// Name: MT_Ellipse constructor
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
MT_Ellipse::MT_Ellipse( const MT_Vector2D& vOrigin, const MT_Vector2D& vMajorAxisHigh, const MT_Vector2D& vMinorAxisHigh )
    : rMajorAxeLength_( vOrigin.Distance( vMajorAxisHigh ) )
    , rMinorAxeLength_( vOrigin.Distance( vMinorAxisHigh ) )
    , rTheta_         ( Angle( MT_Vector2D( 1., 0. ), vMajorAxisHigh - vOrigin ) )
    , rA_             ( rMajorAxeLength_ * rMajorAxeLength_ )
    , rB_             ( rA_ )
    , rC_             ( rMinorAxeLength_ * rMinorAxeLength_ )
    , rF_             ( -rA_ * rC_ )
    , vCenter_        ( vOrigin )
{
    if( rMajorAxeLength_ < rMinorAxeLength_ )
    {
        std::swap( rMajorAxeLength_, rMinorAxeLength_ );
        std::swap( rA_, rC_ );
        rB_ = rA_;
        rTheta_ += MT_PI / 2.;
    }

    assert( rA_ >= rC_ );

    double xc = sqrt( rA_ - rC_ );
    double yc = sin( rTheta_ ) * xc;
    xc *= cos( rTheta_ );

    rA_ -= xc * xc;
    rB_ -= yc * yc;
    rC_ = -2. * xc * yc;
}

//-----------------------------------------------------------------------------
// Name: MT_Ellipse constructor
// Created: JVT 04-02-26
//-----------------------------------------------------------------------------
MT_Ellipse::MT_Ellipse( const MT_Ellipse& ellipse )
    : rMajorAxeLength_( ellipse.rMajorAxeLength_ )
    , rMinorAxeLength_( ellipse.rMinorAxeLength_ )
    , rTheta_         ( ellipse.rTheta_ )
    , rA_             ( ellipse.rA_ )
    , rB_             ( ellipse.rB_ )
    , rC_             ( ellipse.rC_ )
    , rF_             ( ellipse.rF_ )
    , vCenter_        ( ellipse.vCenter_ )
{
}


//-----------------------------------------------------------------------------
// Name: MT_Ellipse destructor
// Created: JVT 04-02-26
//-----------------------------------------------------------------------------
MT_Ellipse::~MT_Ellipse()
{
}


//-----------------------------------------------------------------------------
// Name: MT_Ellipse copy operator
// Created: JVT 04-02-26
//-----------------------------------------------------------------------------
MT_Ellipse& MT_Ellipse::operator = ( const MT_Ellipse& ellipse )
{
    rMajorAxeLength_ = ellipse.rMajorAxeLength_;
    rMinorAxeLength_ = ellipse.rMinorAxeLength_;
    rTheta_          = ellipse.rTheta_;

    rA_      = ellipse.rA_;
    rB_      = ellipse.rB_;
    rC_      = ellipse.rC_;
    rF_      = ellipse.rF_;
    vCenter_ = ellipse.vCenter_;

    return *this;
}

// -----------------------------------------------------------------------------
// Name: MT_Ellipse::GetBoundingBox
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
MT_Rect MT_Ellipse::GetBoundingBox() const
{
    // $$$$ AGE 2005-01-31: Improve it if you dare !
    return MT_Rect( vCenter_.rX_ - rMajorAxeLength_, vCenter_.rY_ - rMajorAxeLength_,
                    vCenter_.rX_ + rMajorAxeLength_, vCenter_.rY_ + rMajorAxeLength_ );
}

//-----------------------------------------------------------------------------
// Name: MT_Ellipse::IsInside
// Created: JVT 04-02-26
//-----------------------------------------------------------------------------
bool MT_Ellipse::IsInside( MT_Vector2D vPoint ) const
{
    vPoint -= vCenter_; // translation du repère pour centrer l'ellipse
    const double rValue = ( rA_ * vPoint.rX_ * vPoint.rX_
                            + rB_ * vPoint.rY_ * vPoint.rY_
                            + rC_ * vPoint.rX_ * vPoint.rY_
                            + rF_ );
    return rValue <= 0.;
}

//-----------------------------------------------------------------------------
// Name: MT_Ellipse::GetCenter
// Created: JVT 04-03-16
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Ellipse::GetCenter() const
{
    return vCenter_;
}

// -----------------------------------------------------------------------------
// Name: MT_Ellipse::GetMajorAxeLength
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
double MT_Ellipse::GetMajorAxeLength() const
{
    return rMajorAxeLength_;
}

//-----------------------------------------------------------------------------
// Name: MT_Ellipse::GetMajorAxisHighPoint
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
const MT_Vector2D MT_Ellipse::GetMajorAxisHighPoint()  const
{
    MT_Vector2D axis( cos( rTheta_ ), sin( rTheta_ ) );
    axis *= rMajorAxeLength_;
    return axis += vCenter_;
}

//-----------------------------------------------------------------------------
// Name: MT_Ellipse::GetMinorAxisHighPoint
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
const MT_Vector2D MT_Ellipse::GetMinorAxisHighPoint()  const
{
    MT_Vector2D axis( -sin( rTheta_ ), cos( rTheta_ ) );
    axis *= rMinorAxeLength_;
    return axis += vCenter_;
}
