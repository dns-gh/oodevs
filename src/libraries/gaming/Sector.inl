// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Author: Jvt $
// $Modtime: 12/04/05 9:58 $
// $Revision: 2 $
// $Workfile: Sector.inl $
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: Sector::IsInCone
// Created: JVT 02-08-30
//-----------------------------------------------------------------------------
inline
bool Sector::IsInCone( const geometry::Point2f& vPos, float rRadius ) const
{
    if( rSemiAngle_ >= pi_ || rSemiAngle_ <= 0 ) // vision sur 360°
        return vPos.SquareDistance( vOrigin_ ) <= ( rRadius * rRadius );

    return vPos.SquareDistance( vOrigin_ ) <= ( rRadius * rRadius )  // test de distance
        && ( rB1_ * vPos.Y() + rA1_ * vPos.X() + rC1_ >= 0 )            // test 1er demi-plan
        && ( rB2_ * vPos.Y() + rA2_ * vPos.X() + rC2_ <= 0 );           // test 2eme demi-plan
}

//-----------------------------------------------------------------------------
// Name: Sector::IsInSector
// Created: JVT 02-09-02
//-----------------------------------------------------------------------------
inline
bool Sector::IsInSector( const geometry::Point2f& vPos ) const
{
    if( rSemiAngle_ >= pi_ || rSemiAngle_ <= 0 )
        return true;

    return ( rB1_ * vPos.Y() + rA1_ * vPos.X() + rC1_ >= 0 )  // test 1er demi-plan
        && ( rB2_ * vPos.Y() + rA2_ * vPos.X() + rC2_ <= 0 ); // test 2eme demi-plan
}

// -----------------------------------------------------------------------------
// Name: Sector::GetDirection
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const geometry::Vector2f& Sector::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: Sector::GetOrigin
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const geometry::Point2f& Sector::GetOrigin() const
{
    return vOrigin_;
}

// -----------------------------------------------------------------------------
// Name: Sector::GetAngle
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
float Sector::GetAngle() const
{
    return rSemiAngle_ * 2.f;
}
