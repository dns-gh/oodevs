// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Sector.h"

// -----------------------------------------------------------------------------
// Name: MT_Sector constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
MT_Sector::MT_Sector( const MT_Vector2D& vOrigin_, const MT_Vector2D& vDirection, double rAngle )
    : vOrigin_   ( vOrigin_ )
    , vDirection_( vDirection )
    , rSemiAngle_( rAngle / 2. )
{
    if( vDirection.IsZero() )
    {   // -> on passe à 360°
        rSemiAngle_ = rSemiAngle_ > 0 ? -rSemiAngle_ : rSemiAngle_ ;
    }
    else
    {
        rSemiAngle_ = rSemiAngle_ > 0 ? rSemiAngle_ : -rSemiAngle_;

        double COS = cos( rSemiAngle_ );
        double SIN = sin( rSemiAngle_ );

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
