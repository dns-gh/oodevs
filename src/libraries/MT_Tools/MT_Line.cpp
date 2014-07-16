// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Line.h"
#include <cassert>

namespace
{
    bool MT_IsSameSigns( double a, double b )
    {
        return a * b >= 0;
    }
}

//-----------------------------------------------------------------------------
// Name: MT_Line constructor
// Created: FBD 02-11-18
//-----------------------------------------------------------------------------
MT_Line::MT_Line()
:   pPosStart_( 0 )
,   pPosEnd_( 0 )
{
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Line constructor
// Created: FBD 02-02-28
//-----------------------------------------------------------------------------
MT_Line::MT_Line( const MT_Vector2D& pointStart, const MT_Vector2D& pointEnd )
:   pPosStart_( &pointStart )
,   pPosEnd_( &pointEnd )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line destructor
// Created: FBD 02-02-28
//-----------------------------------------------------------------------------
MT_Line::~MT_Line()
{
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Line::Intersect2D
// Created: FBD 02-09-11
//-----------------------------------------------------------------------------
E_CollisionState MT_Line::Intersect2D( const MT_Line& line ) const
{

    double rX1 = GetPosStart().rX_;
    double rY1 = GetPosStart().rY_;
    double rX2 = GetPosEnd().rX_;
    double rY2 = GetPosEnd().rY_;

    double rX3 = line.GetPosStart().rX_;
    double rY3 = line.GetPosStart().rY_;
    double rX4 = line.GetPosEnd().rX_;
    double rY4 = line.GetPosEnd().rY_;

    double rA1, rA2, rB1, rB2, rC1, rC2;  // Coefficients of line eqns.
    double r1, r2, r3, r4;                // 'Sign' values

    // Compute rA1, rB1, rC1, where line joining points 1 and 2 is "rA1 rX + rB1 rY + rC1 = 0.f"

    rA1 = rY2 - rY1;
    rB1 = rX1 - rX2;
    rC1 = rX2 * rY1 - rX1 * rY2;

    // Compute r3 and r4.

    r3 = rA1 * rX3 + rB1 * rY3 + rC1;
    r4 = rA1 * rX4 + rB1 * rY4 + rC1;

    /// Check signs of r3 and r4.
    // If both point 3 and point 4 lie on same side of line 1, the line segments do not intersect

    if( !MT_IsZero( r3 ) && !MT_IsZero( r4 ) && MT_IsSameSigns( r3, r4 ) )
    {
        return ( eDontIntersect );
    }

    // Compute a2, b2, c2

    rA2 = rY4 - rY3;
    rB2 = rX3 - rX4;
    rC2 = rX4 * rY3 - rX3 * rY4;

    // Compute r1 and r2

    r1 = rA2 * rX1 + rB2 * rY1 + rC2;
    r2 = rA2 * rX2 + rB2 * rY2 + rC2;

    // Check signs of r1 and r2.
    // If both point 1 and point 2 lie on same side of second line segment, the line segments do not intersect

    if( !MT_IsZero( r1 ) && !MT_IsZero( r2 ) && MT_IsSameSigns( r1, r2 ) )
    {
        return ( eDontIntersect );
    }

    // lines_intersect
    return ( eDoIntersect );
}


#define _Orientation( rUx, rUy, rVx, rVy ) ( (rUx) * (rVy) - (rUy) * (rVx) )
//-----------------------------------------------------------------------------
//  Name  :  MT_Line::IsClipped
// Created: FBD 02-10-04
//-----------------------------------------------------------------------------
bool MT_Line::IsClipped( const MT_Line& Line ) const
{
//    const double rEpsilon = 1e-8;
    const MT_Vector2D& vPosStartSrc = GetPosStart();
    const MT_Vector2D& vPosEndSrc =   GetPosEnd();

    const MT_Vector2D& vPosStartDest = Line.GetPosStart();
    const MT_Vector2D& vPosEndDest =   Line.GetPosEnd();

    const double rUx = vPosEndSrc.rX_ - vPosStartSrc.rX_;
    const double rUy = vPosEndSrc.rY_ - vPosStartSrc.rY_;
    if( _Orientation( vPosStartDest.rX_ - vPosStartSrc.rX_, vPosStartDest.rY_ - vPosStartSrc.rY_, rUx, rUy )
      * _Orientation( vPosEndDest.rX_   - vPosStartSrc.rX_, vPosEndDest.rY_   - vPosStartSrc.rY_, rUx, rUy ) > 0 )
        return false;

    const double rVx = vPosEndDest.rX_ - vPosStartDest.rX_;
    const double rVy = vPosEndDest.rY_ - vPosStartDest.rY_;
    if( _Orientation( vPosStartSrc.rX_ - vPosStartDest.rX_, vPosStartSrc.rY_ - vPosStartDest.rY_, rVx, rVy )
      * _Orientation( vPosEndSrc.rX_   - vPosStartDest.rX_, vPosEndSrc.rY_   - vPosStartDest.rY_, rVx, rVy ) > 0 )
        return false;

    return true;
}

#undef _Orientation


bool  _GEN_IsZero( double rF )
{
    return rF < 1e-4 && rF > -1e-4;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::Intersect2D
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
E_CollisionState MT_Line::Intersect2D( const MT_Line& line, MT_Vector2D& vPosIntersect ) const
{
    const double rX1 = GetPosStart().rX_;
    const double rY1 = GetPosStart().rY_;
    const double rX2 = GetPosEnd().rX_;
    const double rY2 = GetPosEnd().rY_;

    const double rX3 = line.GetPosStart().rX_;
    const double rY3 = line.GetPosStart().rY_;
    const double rX4 = line.GetPosEnd().rX_;
    const double rY4 = line.GetPosEnd().rY_;

    // Compute rA1, rB1, rC1, where line joining points 1 and 2 is "rA1 rX + rB1 rY + rC1 = 0.f"
    const double rA1 = rY2 - rY1;
    const double rB1 = rX1 - rX2;
    const double rC1 = rX2 * rY1 - rX1 * rY2;

    // Compute r3 and r4.
    const double r3 = rA1 * rX3 + rB1 * rY3 + rC1;
    const double r4 = rA1 * rX4 + rB1 * rY4 + rC1;

    /// Check signs of r3 and r4.
    // If both point 3 and point 4 lie on same side of line 1, the line segments do not intersect
    if( !_GEN_IsZero( r3 ) && !_GEN_IsZero( r4 ) && MT_IsSameSigns( r3, r4 ) )
        return ( eDontIntersect );

    // Compute a2, b2, c2

    const double rA2 = rY4 - rY3;
    const double rB2 = rX3 - rX4;
    const double rC2 = rX4 * rY3 - rX3 * rY4;

    // Compute r1 and r2
    const double r1 = rA2 * rX1 + rB2 * rY1 + rC2;
    const double r2 = rA2 * rX2 + rB2 * rY2 + rC2;

    // Check signs of r1 and r2.
    // If both point 1 and point 2 lie on same side of second line segment, the line segments do not intersect
    if( !_GEN_IsZero( r1 ) && !_GEN_IsZero( r2 ) && MT_IsSameSigns( r1, r2 ) )
        return ( eDontIntersect );

    // Line segments intersect: compute intersection point
    const double rDenom = rA1 * rB2 - rA2 * rB1;
    if( _GEN_IsZero( rDenom ) )
        return ( eCollinear );


    // The rDenom/2 is to get rounding instead of truncating. It is added or subtracted to the numerator, depending upon the sign of the numerator.
    const double rNumX = rB1 * rC2 - rB2 * rC1;
    const double rNumY = rA2 * rC1 - rA1 * rC2;
    const double rInvDenom = double( 1 ) / rDenom;
    vPosIntersect.rX_ = rNumX * rInvDenom;
    vPosIntersect.rY_ = rNumY * rInvDenom;
    // lines_intersect
    return ( eDoIntersect );
}

//-----------------------------------------------------------------------------
// Name: MT_Line::=( const
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
MT_Line& MT_Line::operator = ( const MT_Line& rhs )
{
    pPosStart_ = rhs.pPosStart_;
    pPosEnd_= rhs.pPosEnd_;
    return *this;
}



//-----------------------------------------------------------------------------
// Name: MT_Line::Intersect2D
// Created: NLD 2003-07-24
//-----------------------------------------------------------------------------
bool MT_Line::Intersect2D( const T_PointVector& polyline, T_PointSet& orderedIntersectionSet, double rPrecision ) const
{
    // Return the intersections between the line and the polyline, ordered

    CIT_PointVector itFirstPoint = polyline.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    for( CIT_PointVector itPoint = ++itFirstPoint; itPoint != polyline.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        MT_Vector2D vIntersection;
        bool bFlag = false;
        if( lineTmp.IsInside( *pPosStart_, rPrecision ) )
        {
            bFlag = true;
            orderedIntersectionSet.insert( *pPosStart_ );
        }
        if( lineTmp.IsInside( *pPosEnd_, rPrecision ) )
        {
            bFlag = true;
            orderedIntersectionSet.insert( *pPosEnd_ );
        }

        if( !bFlag && Intersect2D( lineTmp, vIntersection ) == eDoIntersect )
        {
            assert( IsInside( vIntersection, rPrecision ) );
            orderedIntersectionSet.insert( vIntersection );
        }
        pPos1 = pPos2;
    }
    return !orderedIntersectionSet.empty();
}


//-----------------------------------------------------------------------------
// Name: MT_Line::Intersect2D
// Created: NLD 2003-07-25
//-----------------------------------------------------------------------------
bool MT_Line::Intersect2D( const T_PointVector& polyline, double rPrecision ) const
{
    CIT_PointVector itFirstPoint = polyline.begin();
    const MT_Vector2D* pPos1 = &*itFirstPoint;
    for( CIT_PointVector itPoint = ++itFirstPoint; itPoint != polyline.end(); ++itPoint )
    {
        const MT_Vector2D* pPos2 = &*itPoint;
        MT_Line lineTmp( *pPos1, *pPos2 );

        MT_Vector2D vIntersection;
        if( lineTmp.IsInside( *pPosStart_, rPrecision ) )
            return true;
        else if( lineTmp.IsInside( *pPosEnd_, rPrecision ) )
            return true;
        else if( Intersect2D( lineTmp ) == eDoIntersect ) // ?? != eDontIntersect
            return true;
        pPos1 = pPos2;
    }
    return false;
}

//*****************************************************************************
// Name   : MT_Line.inl
// Created: FBD 02-02-27
//*****************************************************************************

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::GetPosStart
// Created: FBD 02-02-28
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Line::GetPosStart() const
{
    return *pPosStart_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::GetPosEnd
// Created: FBD 02-02-28
//-----------------------------------------------------------------------------
const MT_Vector2D& MT_Line::GetPosEnd() const
{
    return *pPosEnd_;
}

//-----------------------------------------------------------------------------
// Name: MT_Line::GetCenter
// Created: FBD 02-12-10
//-----------------------------------------------------------------------------
MT_Vector2D MT_Line::GetCenter() const
{
    return ( GetPosStart() + GetPosEnd() ) / 2.0;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Line::IsInside
// Created: FBD 02-08-21
//-----------------------------------------------------------------------------
bool MT_Line::IsInside( const MT_Vector2D& vPos, double rSize ) const
{
    static const double rEpsilon = 1e-8;
    MT_Vector2D vPosNear = ClosestPointOnLine( vPos );
    double rDist = vPos.SquareDistance( vPosNear );
    double rDist2 = rSize * rSize;
    return( rDist2 - rDist >= -rEpsilon );
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::IsInside
// Created: FBD 02-08-21
//-----------------------------------------------------------------------------
bool MT_Line::IsInside( const MT_Vector2D& vPos, double rSize, MT_Vector2D& vPosNear ) const
{
    vPosNear = ClosestPointOnLine( vPos );
    return( vPos.SquareDistance( vPosNear ) < ( rSize * rSize ) );
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::ClosestPointOnLine
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
MT_Vector2D MT_Line::ClosestPointOnLine( const MT_Vector2D& vPoint ) const
{
    const double rLenDeltaSqr = pPosStart_->SquareDistance( *pPosEnd_ );
    if( MT_IsZero( rLenDeltaSqr ) )
        return *pPosStart_;

    const MT_Vector2D vDeltaPoint = vPoint - *pPosStart_;
    const MT_Vector2D vDelta = *pPosEnd_ - *pPosStart_;

    const double rDot = DotProduct( vDelta, vDeltaPoint );
    if ( rDot <= 0.f ) 
        return *pPosStart_;

    if ( rDot >= rLenDeltaSqr )
        return *pPosEnd_;

    return *pPosStart_ + vDelta * ( rDot / rLenDeltaSqr );
}

// -----------------------------------------------------------------------------
// Name: MT_Line::DistancePointToLine
/*
 *    Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
    Let P be the point of perpendicular projection of C on AB.  The parameter
    r, which indicates P's position along AB, is computed by the dot product 
    of AC and AB divided by the square of the length of AB:
    
    (1)     AC dot AB
        r = ---------  
            ||AB||^2
    
    r has the following meaning:
    
        r=0      P = A
        r=1      P = B
        r<0      P is on the backward extension of AB
        r>1      P is on the forward extension of AB
        0<r<1    P is interior to AB

 */
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
MT_Vector2D MT_Line::ProjectPointOnLine( const MT_Vector2D& vPoint ) const
{
    if( *pPosEnd_ == *pPosStart_ )
        return *pPosStart_;

    MT_Vector2D vAB( *pPosEnd_ - *pPosStart_ );
    MT_Vector2D vAC( vPoint    - *pPosStart_ );

    double r = DotProduct( vAC, vAB ) / vAB.SquareMagnitude();

    return *pPosStart_ + vAB * r;
}

// -----------------------------------------------------------------------------
// Name: MT_Line::ProjectPointOnLine
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
double MT_Line::ProjectPointOnLine( const MT_Vector2D& vPoint, MT_Vector2D& vResult ) const
{
    if( *pPosEnd_ == *pPosStart_ )
    {
        vResult = *pPosStart_;
        return 0.;
    }

    MT_Vector2D vAB( *pPosEnd_ - *pPosStart_ );
    MT_Vector2D vAC( vPoint    - *pPosStart_ );

    double r = DotProduct( vAC, vAB ) / vAB.SquareMagnitude();

    vResult = *pPosStart_ + vAB * r;
    
    return r;
}

// -----------------------------------------------------------------------------
// Name: MT_Line::Magnitude
// Created: NLD 2003-10-17
// -----------------------------------------------------------------------------
double MT_Line::Magnitude() const
{
    return ( *pPosEnd_ - *pPosStart_ ).Magnitude();
}
