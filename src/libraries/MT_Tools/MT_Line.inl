//*****************************************************************************
// Name   : MT_Line.inl
// Created: FBD 02-02-27
//*****************************************************************************

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::GetPosStart
// Created: FBD 02-02-28
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Line::GetPosStart() const
{
    return *pPosStart_;
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::GetPosEnd
// Created: FBD 02-02-28
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Line::GetPosEnd() const
{
    return *pPosEnd_;
}

//-----------------------------------------------------------------------------
// Name: MT_Line::GetCenter
// Created: FBD 02-12-10
//-----------------------------------------------------------------------------
inline
MT_Vector2D MT_Line::GetCenter() const
{
    return ( GetPosStart() + GetPosEnd() ) / 2.0;
}


//-----------------------------------------------------------------------------
//  Name  :  MT_Line::IsInside
// Created: FBD 02-08-21
//-----------------------------------------------------------------------------
inline
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
inline
bool MT_Line::IsInside( const MT_Vector2D& vPos, double rSize, MT_Vector2D& vPosNear ) const
{
    vPosNear = ClosestPointOnLine( vPos );
    return( vPos.SquareDistance( vPosNear ) < ( rSize * rSize ) );
}

//-----------------------------------------------------------------------------
//  Name  :  MT_Line::ClosestPointOnLine
// Created: FBD 02-03-01
//-----------------------------------------------------------------------------
inline
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
inline
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
inline
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
inline
double MT_Line::Magnitude() const
{
    return ( *pPosEnd_ - *pPosStart_ ).Magnitude();
}
