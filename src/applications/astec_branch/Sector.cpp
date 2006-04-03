// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/Sector.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 10:00 $
// $Revision: 2 $
// $Workfile: Sector.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "Sector.h"

const float Sector::pi_ = std::acos( -1.f );

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: Sector constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
Sector::Sector( const Point2f& vOrigin, const Vector2f& vDirection, float rAngle )
    : vOrigin_   ( vOrigin )
    , vDirection_( vDirection )
    , rSemiAngle_( rAngle / 2. )
{
    if ( vDirection.IsNull() )
    {   // -> on passe à 360°
        rSemiAngle_ = rSemiAngle_ > 0 ? -rSemiAngle_ : rSemiAngle_ ;
    }
    else
    {
        rSemiAngle_ = rSemiAngle_ > 0 ? rSemiAngle_ : -rSemiAngle_;

        float COS = std::cos( rSemiAngle_ );
        float SIN = std::sin( rSemiAngle_ );

        rA1_ = vDirection.Y() * COS + vDirection.X() * SIN;
        rB1_ = vDirection.Y() * SIN - vDirection.X() * COS;
        rC1_ = -( rA1_ * vOrigin_.X() + rB1_ * vOrigin_.Y() );

        rA2_ = vDirection.Y() * COS - vDirection.X() * SIN;
        rB2_ = -vDirection.Y() * SIN - vDirection.X() * COS;
        rC2_ = -( rA2_ * vOrigin_.X() + rB2_ * vOrigin_.Y() );    
    }
}

// -----------------------------------------------------------------------------
// Name: Sector constructor
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
Sector::Sector()
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sector destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
Sector::~Sector()
{
    // NOTHING
}
