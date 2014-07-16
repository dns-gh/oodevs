// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Sector.inl $
// $Author: Jvt $
// $Modtime: 12/04/05 9:58 $
// $Revision: 2 $
// $Workfile: MT_Sector.inl $
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
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_Sector::IsInCone
// Created: JVT 02-08-30
//-----------------------------------------------------------------------------
bool MT_Sector::IsInCone( const MT_Vector2D& vPos, double rRadius ) const
{
    if ( rSemiAngle_ >= MT_PI || rSemiAngle_ <= 0 ) // vision sur 360°
        return vPos.SquareDistance( vOrigin_ ) <= ( rRadius * rRadius );

    return vPos.SquareDistance( vOrigin_ ) <= ( rRadius * rRadius )  // test de distance
        && ( rB1_ * vPos.rY_ + rA1_ * vPos.rX_ + rC1_ >= 0 )            // test 1er demi-plan
        && ( rB2_ * vPos.rY_ + rA2_ * vPos.rX_ + rC2_ <= 0 );           // test 2eme demi-plan  
}



//-----------------------------------------------------------------------------
// Name: MT_Sector::IsInSector
// Created: JVT 02-09-02
//-----------------------------------------------------------------------------
bool MT_Sector::IsInSector( const MT_Vector2D& vPos ) const
{
    if ( rSemiAngle_ >= MT_PI || rSemiAngle_ <= 0 )
        return true;

    return ( rB1_ * vPos.rY_ + rA1_ * vPos.rX_ + rC1_ >= 0 )  // test 1er demi-plan
        && ( rB2_ * vPos.rY_ + rA2_ * vPos.rX_ + rC2_ <= 0 ); // test 2eme demi-plan  
}

// -----------------------------------------------------------------------------
// Name: MT_Sector::GetDirection
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const MT_Vector2D& MT_Sector::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: MT_Sector::GetOrigin
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const MT_Vector2D& MT_Sector::GetOrigin() const
{
    return vOrigin_;
}

// -----------------------------------------------------------------------------
// Name: MT_Sector::GetAngle
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
double MT_Sector::GetAngle() const
{
    return rSemiAngle_ * 2;
}
