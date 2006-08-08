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

#include "astec_gaming_pch.h"
#include "Sector.h"
#include "astec_kernel/GlTools_ABC.h"

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

    direction1_ = direction2_ = vDirection_;
    direction1_.Normalize(); direction2_.Normalize();
    const float rSin = std::sin( rSemiAngle_ );
    const float rCos = std::cos( rSemiAngle_ );
    direction1_ = Vector2f( direction1_.X() * rCos + direction1_.Y() * rSin
                          , direction1_.Y() * rCos - direction1_.X() * rSin );
    direction2_ = Vector2f( direction2_.X() * rCos - direction2_.Y() * rSin
                          , direction2_.Y() * rCos + direction2_.X() * rSin );

    float A0 = std::acos( vDirection_.X() );
    if( std::asin( vDirection_.Y() ) < 0 )
        A0 = -A0;
    minAngle_ = A0 - rSemiAngle_;
    maxAngle_ = A0 + rSemiAngle_;
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

// -----------------------------------------------------------------------------
// Name: Sector::Draw
// Created: AGE 2006-04-26
// -----------------------------------------------------------------------------
void Sector::Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools, float radius ) const
{
    if( vOrigin_.IsZero() )
        tools.DrawCircle( vOrigin_, radius );
    else
    {
        const Vector2f dir1 = direction1_ * radius;
        const Vector2f dir2 = direction2_ * radius;

        tools.DrawLine( vOrigin_, vOrigin_ + dir1 );
        tools.DrawLine( vOrigin_, vOrigin_ + dir2 );
        tools.DrawArc( vOrigin_, vOrigin_ + dir1, vOrigin_ + dir2 );
    }
}