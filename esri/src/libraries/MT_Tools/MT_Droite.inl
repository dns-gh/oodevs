// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Droite.inl $
// $Author: Nld $
// $Modtime: 1/09/04 16:44 $
// $Revision: 4 $
// $Workfile: MT_Droite.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MT_Droite::GetDistanceToPoint
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
inline
MT_Float MT_Droite::GetDistanceToPoint( const MT_Vector2D& vPoint ) const
{
    MT_Float rTmp = sqrt( ( rA_ * rA_ ) + ( rB_ * rB_ ) );
    if( rTmp == 0.0f )
        return 0.0f;
    return ( fabs( rA_ * vPoint.rX_ + rB_ * vPoint.rY_ + rC_ ) ) / rTmp;
}

//-----------------------------------------------------------------------------
// Name: MT_Droite::GetSide
// Created: JVT 04-03-17
//-----------------------------------------------------------------------------
inline
MT_Droite::E_Side MT_Droite::GetSide( MT_Float rX, MT_Float rY )  const
{
    MT_Float rTmp = rX * rA_ + rY * rB_ + rC_;

    return MT_IsZero( rTmp ) ? eOnBoundary : rTmp < 0 ? eOnNegativeSide : eOnPositiveSide;
}
