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

//-----------------------------------------------------------------------------
// Name: MT_Sector::IsInCone
// Created: JVT 02-08-30
//-----------------------------------------------------------------------------
inline
bool MT_Sector::IsInCone( const MT_Vector2D& vPos, MT_Float rRadius ) const
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
inline
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
inline
const MT_Vector2D& MT_Sector::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: MT_Sector::GetOrigin
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MT_Sector::GetOrigin() const
{
    return vOrigin_;
}

// -----------------------------------------------------------------------------
// Name: MT_Sector::GetAngle
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
MT_Float MT_Sector::GetAngle() const
{
    return rSemiAngle_ * 2;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MT_Sector::serialize
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_Sector::serialize( Archive& file, const uint )
{
    file & vOrigin_
         & vDirection_
         & rSemiAngle_
         & rA1_
         & rB1_
         & rC1_
         & rA2_
         & rB2_
         & rC2_;
}
