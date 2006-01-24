//*****************************************************************************
//
// $Created: JVT 04-02-26 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Ellipse.cpp $
// $Author: Age $
// $Modtime: 2/03/05 17:46 $
// $Revision: 10 $
// $Workfile: MT_Ellipse.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Ellipse.h"

MT_BOOSTPOOLING_FUNCTIONS( MT_Ellipse )

//-----------------------------------------------------------------------------
// Name: MT_Ellipse constructor
// Created: JVT 04-02-26
// Last modified: JVT 04-03-25
//-----------------------------------------------------------------------------
MT_Ellipse::MT_Ellipse( const MT_Vector2D& vOrigin, MT_Float rMajorAxeLength, MT_Float rMinorAxeLength, MT_Float rTheta )
    : rMajorAxeLength_( rMajorAxeLength )
    , rMinorAxeLength_( rMinorAxeLength )
    , rTheta_         ( rTheta )
    , rA_             ( rMajorAxeLength_ * rMajorAxeLength_ )
    , rB_             ( rA_ )
    , rC_             ( rMinorAxeLength_ * rMinorAxeLength_ )
    , rF_             ( -rA_ * rC_ )
    , vCenter_        ( vOrigin )
{
    if ( rMajorAxeLength_ < rMinorAxeLength_ )
    {
        std::swap( rMajorAxeLength_, rMinorAxeLength_ );
        std::swap( rA_, rC_ );
        rB_ = rA_;
        rTheta_ += MT_PI / 2.;
    }

    assert( rA_ >= rC_ );

    MT_Float xc = sqrt( rA_ - rC_ );
    MT_Float yc = sin( rTheta_ ) * xc;
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
    if ( rMajorAxeLength_ < rMinorAxeLength_ )
    {
        std::swap( rMajorAxeLength_, rMinorAxeLength_ );
        std::swap( rA_, rC_ );
        rB_ = rA_;
        rTheta_ += MT_PI / 2.;
    }

    assert( rA_ >= rC_ );

    MT_Float xc = sqrt( rA_ - rC_ );
    MT_Float yc = sin( rTheta_ ) * xc;
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
    const MT_Float rValue = ( rA_ * vPoint.rX_ * vPoint.rX_
                            + rB_ * vPoint.rY_ * vPoint.rY_
                            + rC_ * vPoint.rX_ * vPoint.rY_
                            + rF_ );
    return rValue <= 0.;
}
