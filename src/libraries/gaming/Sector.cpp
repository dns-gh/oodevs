// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Author: Jvt $
// $Modtime: 12/04/05 10:00 $
// $Revision: 2 $
// $Workfile: Sector.cpp $
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Sector.h"
#include "clients_gui/GLView_ABC.h"

const float Sector::pi_ = std::acos( -1.f );

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Sector constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
Sector::Sector( const Point2f& vOrigin, unsigned heading, float rAngle )
    : vOrigin_   ( vOrigin )
    , vDirection_( std::sin( heading * pi_ / 180.f ), std::cos( heading * pi_ / 180.f ) )
    , rSemiAngle_( rAngle / 2.f )
{
    if( vDirection_.IsNull() )
    {   // -> on passe à 360°
        rSemiAngle_ = rSemiAngle_ > 0 ? -rSemiAngle_ : rSemiAngle_ ;
    }
    else
    {
        rSemiAngle_ = rSemiAngle_ > 0 ? rSemiAngle_ : -rSemiAngle_;

        float COS = std::cos( rSemiAngle_ );
        float SIN = std::sin( rSemiAngle_ );

        rA1_ = vDirection_.Y() * COS + vDirection_.X() * SIN;
        rB1_ = vDirection_.Y() * SIN - vDirection_.X() * COS;
        rC1_ = -( rA1_ * vOrigin_.X() + rB1_ * vOrigin_.Y() );

        rA2_ = vDirection_.Y() * COS - vDirection_.X() * SIN;
        rB2_ = -vDirection_.Y() * SIN - vDirection_.X() * COS;
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
    : vOrigin_()
    , vDirection_()
    , rSemiAngle_( 0. )
    , rA1_( 0. )
    , rB1_( 0. )
    , rC1_( 0. )
    , rA2_( 0. )
    , rB2_( 0. )
    , rC2_( 0. )
    , minAngle_( 0. )
    , maxAngle_( 0. )
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
void Sector::Draw( const gui::GLView_ABC& tools, float radius ) const
{
    if( vDirection_.IsNull() )
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

// -----------------------------------------------------------------------------
// Name: Sector::DrawFill
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void Sector::DrawFill( float radius ) const
{
    glBegin( GL_TRIANGLE_FAN );
        glTexCoord1f( 0.125f );
        glVertex3f( vOrigin_.X(), vOrigin_.Y(), 200  );
        for( float angle = minAngle_; angle < maxAngle_; angle += 0.05f )
        {
            geometry::Point2f point = vOrigin_ + radius * geometry::Vector2f( std::cos( angle ), std::sin( angle ) );
            glTexCoord1f( 1.f -  0.125f );
            glVertex3f( point.X(), point.Y(), 0 );
        }
    glEnd();
}
