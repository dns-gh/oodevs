// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Sector.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 10:00 $
// $Revision: 2 $
// $Workfile: MT_Sector.cpp $
//
// *****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_Sector.h"

//MT_BOOSTPOOLING_FUNCTIONS( MT_Sector )

// -----------------------------------------------------------------------------
// Name: MT_Sector constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
MT_Sector::MT_Sector( const MT_Vector2D& vOrigin_, const MT_Vector2D& vDirection, MT_Float rAngle )
    : vOrigin_   ( vOrigin_ )
    , vDirection_( vDirection )
    , rSemiAngle_( rAngle / 2. )
{
    if ( vDirection.IsZero() )
    {   // -> on passe à 360°
        rSemiAngle_ = rSemiAngle_ > 0 ? -rSemiAngle_ : rSemiAngle_ ;
    }
    else
    {
        rSemiAngle_ = rSemiAngle_ > 0 ? rSemiAngle_ : -rSemiAngle_;

        MT_Float COS = cos( rSemiAngle_ );
        MT_Float SIN = sin( rSemiAngle_ );

        rA1_ = vDirection.rY_ * COS + vDirection.rX_ * SIN;
        rB1_ = vDirection.rY_ * SIN - vDirection.rX_ * COS;
        rC1_ = -( rA1_ * vOrigin_.rX_ + rB1_ * vOrigin_.rY_ );

        rA2_ = vDirection.rY_ * COS - vDirection.rX_ * SIN;
        rB2_ = -vDirection.rY_ * SIN - vDirection.rX_ * COS;
        rC2_ = -( rA2_ * vOrigin_.rX_ + rB2_ * vOrigin_.rY_ );    
    }
}

// -----------------------------------------------------------------------------
// Name: MT_Sector constructor
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
MT_Sector::MT_Sector()
    : vOrigin_   ()
    , vDirection_()
    , rSemiAngle_( 0. )
    , rA1_       ( 0. )
    , rB1_       ( 0. )
    , rC1_       ( 0. )
    , rA2_       ( 0. )
    , rB2_       ( 0. )
    , rC2_       ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: MT_Sector destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
MT_Sector::~MT_Sector()
{
}
