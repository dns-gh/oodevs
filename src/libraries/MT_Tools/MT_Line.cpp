//*****************************************************************************
// Name   : MT_Line.cpp
// Created: FBD 02-02-27
//*****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_Line.h"


#ifdef  _DEBUG
#   undef   INLINE                  
#   define  INLINE
#   include "MT_Line.inl"
#endif  // _DEBUG
          

MT_BOOSTPOOLING_FUNCTIONS( MT_Line )

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
   
    MT_Float rX1 = GetPosStart().rX_;
    MT_Float rY1 = GetPosStart().rY_;
    MT_Float rX2 = GetPosEnd().rX_;
    MT_Float rY2 = GetPosEnd().rY_;

    MT_Float rX3 = line.GetPosStart().rX_;
    MT_Float rY3 = line.GetPosStart().rY_;
    MT_Float rX4 = line.GetPosEnd().rX_;
    MT_Float rY4 = line.GetPosEnd().rY_;

    MT_Float rA1, rA2, rB1, rB2, rC1, rC2;  // Coefficients of line eqns.
    MT_Float r1, r2, r3, r4;                // 'Sign' values

    // Compute rA1, rB1, rC1, where line joining points 1 and 2 is "rA1 rX + rB1 rY + rC1 = 0.f"

    rA1 = rY2 - rY1;
    rB1 = rX1 - rX2;
    rC1 = rX2 * rY1 - rX1 * rY2;

    // Compute r3 and r4.

    r3 = rA1 * rX3 + rB1 * rY3 + rC1;
    r4 = rA1 * rX4 + rB1 * rY4 + rC1;

    /// Check signs of r3 and r4. 
    // If both point 3 and point 4 lie on same side of line 1, the line segments do not intersect

    if ( !MT_IsZero( r3 ) && !MT_IsZero( r4 ) && MT_IsSameSigns( r3, r4 ) )
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

    if ( !MT_IsZero( r1 ) && !MT_IsZero( r2 ) && MT_IsSameSigns( r1, r2 ) )
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
//    const MT_Float rEpsilon = 1e-8;
    const MT_Vector2D& vPosStartSrc = GetPosStart();
    const MT_Vector2D& vPosEndSrc =   GetPosEnd();

    const MT_Vector2D& vPosStartDest = Line.GetPosStart();
    const MT_Vector2D& vPosEndDest =   Line.GetPosEnd();

    const MT_Float rUx = vPosEndSrc.rX_ - vPosStartSrc.rX_;
    const MT_Float rUy = vPosEndSrc.rY_ - vPosStartSrc.rY_;
    if( _Orientation( vPosStartDest.rX_ - vPosStartSrc.rX_, vPosStartDest.rY_ - vPosStartSrc.rY_, rUx, rUy )
      * _Orientation( vPosEndDest.rX_   - vPosStartSrc.rX_, vPosEndDest.rY_   - vPosStartSrc.rY_, rUx, rUy ) > 0 )
        return false;

    const MT_Float rVx = vPosEndDest.rX_ - vPosStartDest.rX_;
    const MT_Float rVy = vPosEndDest.rY_ - vPosStartDest.rY_;
    if( _Orientation( vPosStartSrc.rX_ - vPosStartDest.rX_, vPosStartSrc.rY_ - vPosStartDest.rY_, rVx, rVy )
      * _Orientation( vPosEndSrc.rX_   - vPosStartDest.rX_, vPosEndSrc.rY_   - vPosStartDest.rY_, rVx, rVy ) > 0 )
        return false;

    return true;
}

#undef _Orientation


inline
bool  _GEN_IsZero( MT_Float rF )
{ 
    return rF < 1e-4 && rF > -1e-4;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::Intersect2D
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
E_CollisionState MT_Line::Intersect2D( const MT_Line& line, MT_Vector2D& vPosIntersect ) const
{
    const MT_Float rX1 = GetPosStart().rX_;
    const MT_Float rY1 = GetPosStart().rY_;
    const MT_Float rX2 = GetPosEnd().rX_;
    const MT_Float rY2 = GetPosEnd().rY_;

    const MT_Float rX3 = line.GetPosStart().rX_;
    const MT_Float rY3 = line.GetPosStart().rY_;
    const MT_Float rX4 = line.GetPosEnd().rX_;
    const MT_Float rY4 = line.GetPosEnd().rY_;

    // Compute rA1, rB1, rC1, where line joining points 1 and 2 is "rA1 rX + rB1 rY + rC1 = 0.f"
    const MT_Float rA1 = rY2 - rY1;
    const MT_Float rB1 = rX1 - rX2;
    const MT_Float rC1 = rX2 * rY1 - rX1 * rY2;

    // Compute r3 and r4.
    const MT_Float r3 = rA1 * rX3 + rB1 * rY3 + rC1;
    const MT_Float r4 = rA1 * rX4 + rB1 * rY4 + rC1;

    /// Check signs of r3 and r4. 
    // If both point 3 and point 4 lie on same side of line 1, the line segments do not intersect
    if ( !_GEN_IsZero( r3 ) && !_GEN_IsZero( r4 ) && MT_IsSameSigns( r3, r4 ) )
        return ( eDontIntersect );
        
    // Compute a2, b2, c2

    const MT_Float rA2 = rY4 - rY3;
    const MT_Float rB2 = rX3 - rX4;
    const MT_Float rC2 = rX4 * rY3 - rX3 * rY4;

    // Compute r1 and r2
    const MT_Float r1 = rA2 * rX1 + rB2 * rY1 + rC2;
    const MT_Float r2 = rA2 * rX2 + rB2 * rY2 + rC2;

    // Check signs of r1 and r2.
    // If both point 1 and point 2 lie on same side of second line segment, the line segments do not intersect
    if ( !_GEN_IsZero( r1 ) && !_GEN_IsZero( r2 ) && MT_IsSameSigns( r1, r2 ) )
        return ( eDontIntersect );
        
    // Line segments intersect: compute intersection point
    const MT_Float rDenom = rA1 * rB2 - rA2 * rB1;
    if ( _GEN_IsZero( rDenom ) )
        return ( eCollinear );
        

    // The rDenom/2 is to get rounding instead of truncating. It is added or subtracted to the numerator, depending upon the sign of the numerator.
    const MT_Float rNumX = rB1 * rC2 - rB2 * rC1;
    const MT_Float rNumY = rA2 * rC1 - rA1 * rC2;
    const MT_Float rInvDenom = MT_Float( 1 ) / rDenom;
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
bool MT_Line::Intersect2D( const T_PointVector& polyline, T_PointSet& orderedIntersectionSet, MT_Float rPrecision ) const
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
bool MT_Line::Intersect2D( const T_PointVector& polyline, MT_Float rPrecision ) const
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

