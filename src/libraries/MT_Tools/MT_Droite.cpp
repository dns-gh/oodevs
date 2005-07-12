// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Droite.cpp $
// $Author: Jvt $
// $Modtime: 14/02/05 11:24 $
// $Revision: 5 $
// $Workfile: MT_Droite.cpp $
//
// *****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_Droite.h"

#include "MT_Line.h"

// -----------------------------------------------------------------------------
// Name: MT_Droite constructor
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
MT_Droite::MT_Droite()
: rA_( 0 )
, rB_( 0 )
, rC_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: MT_Droite constructor
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
MT_Droite::MT_Droite( const MT_Vector2D& vPointInLine1, const MT_Vector2D& vPointInLine2 )
    : rA_( vPointInLine2.rY_ - vPointInLine1.rY_ )
    , rB_( vPointInLine1.rX_ - vPointInLine2.rX_ )
    , rC_( vPointInLine2.rX_ * vPointInLine1.rY_ - vPointInLine1.rX_ * vPointInLine2.rY_ )
{
}

//-----------------------------------------------------------------------------
// Name: MT_Droite constructor
// Created: JVT 04-03-17
//-----------------------------------------------------------------------------
MT_Droite::MT_Droite( MT_Float rX1, MT_Float rY1, MT_Float rX2, MT_Float rY2 )
    : rA_( rY2 - rY1 )
    , rB_( rX1 - rX2 )
    , rC_( rX2 * rY1 - rX1 * rY2 )
{
}


//-----------------------------------------------------------------------------
// Name: MT_Droite constructor
// Created: JVT 04-03-17
//-----------------------------------------------------------------------------
MT_Droite::MT_Droite( const MT_Droite& rhs )
    : rA_ ( rhs.rA_ )
    , rB_ ( rhs.rB_ )
    , rC_ ( rhs.rC_ )
{
}


//-----------------------------------------------------------------------------
// Name: MT_Droite::operator =
// Created: JVT 04-03-17
//-----------------------------------------------------------------------------
MT_Droite& MT_Droite::operator = ( const MT_Droite& rhs )
{
    rA_ = rhs.rA_;
    rB_ = rhs.rB_;
    rC_ = rhs.rC_;

    return *this;
}


// -----------------------------------------------------------------------------
// Name: MT_Droite destructor
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
MT_Droite::~MT_Droite()
{

}

// ==========================================================================b===
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MT_Droite::GetSide
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
MT_Droite::E_Side MT_Droite::GetSide( const MT_Vector2D& vPoint ) const
{
    MT_Float rResult = vPoint.rX_ * rA_ + vPoint.rY_ * rB_ + rC_;
    if( MT_IsZero( rResult ) )
        return eOnBoundary;

    if( rResult < 0 )
        return eOnNegativeSide;
    else
        return eOnPositiveSide;
}

// -----------------------------------------------------------------------------
// Name: MT_Droite::IsInside
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
bool MT_Droite::IsInside( const MT_Vector2D& vPoint ) const
{
    return GetSide( vPoint ) == eOnBoundary;
}
                  
// -----------------------------------------------------------------------------
// Name: MT_Droite::Intersect2D
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
E_CollisionState MT_Droite::Intersect2D( const MT_Line& segment, MT_Vector2D& vPosIntersect ) const
{
    vPosIntersect.Reset();
    
    // Test if segment intersect with the line
    const MT_Float r3 = segment.GetPosStart().rX_ * rA_ + segment.GetPosStart().rY_ * rB_ + rC_;
    const MT_Float r4 = segment.GetPosEnd  ().rX_ * rA_ + segment.GetPosEnd  ().rY_ * rB_ + rC_;

    /// Check signs of r3 and r4. 
    // If both point 3 and point 4 lie on same side of line 1, the line segments do not intersect
    if ( !MT_IsZero( r3 ) && !MT_IsZero( r4 ) && MT_IsSameSigns( r3, r4 ) )
        return eDontIntersect;

    // Compute intersection
    // Compute rA2, rB2, rC2 : equation of segment
    const MT_Float rA2 = segment.GetPosEnd  ().rY_ - segment.GetPosStart().rY_;
    const MT_Float rB2 = segment.GetPosStart().rX_ - segment.GetPosEnd  ().rX_;
    const MT_Float rC2 = segment.GetPosEnd  ().rX_ * segment.GetPosStart().rY_ - segment.GetPosStart().rX_ * segment.GetPosEnd().rY_;

    const MT_Float rDenom = rA_ * rB2 - rA2 * rB_;
    if ( MT_IsZero( rDenom ) )
        return eCollinear;

    vPosIntersect.rX_ = rB_ * rC2 - rB2 * rC_;
    vPosIntersect.rY_ = rA2 * rC_ - rA_ * rC2;
    vPosIntersect /= rDenom;

    assert( GetSide( vPosIntersect ) == eOnBoundary );
    return eDoIntersect;
}

// -----------------------------------------------------------------------------
// Name: MT_Droite::Intersect2D
// Created: JVT 2005-02-14
// -----------------------------------------------------------------------------
E_CollisionState MT_Droite::Intersect2D( const MT_Droite& droite, MT_Vector2D& vPosIntersect ) const
{
    vPosIntersect.Reset();
            
    const MT_Float rDenom = rA_ * droite.rB_ - droite.rA_ * rB_;
    if ( MT_IsZero( rDenom ) )
        return eCollinear;

    vPosIntersect.rX_ = rB_ * droite.rC_ - droite.rB_ * rC_;
    vPosIntersect.rY_ = rC_ * droite.rA_ - droite.rC_ * rA_;
    vPosIntersect /= rDenom;

    assert( GetSide( vPosIntersect ) == eOnBoundary );
    return eDoIntersect;
}

