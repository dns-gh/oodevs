//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Limit/MIL_Limit.inl $
// $Author: Nld $
// $Modtime: 21/10/04 15:50 $
// $Revision: 2 $
// $Workfile: MIL_Limit.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MIL_Limit::GetID
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
uint MIL_Limit::GetID() const
{
    return nID_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Limit::GetPointVector
// Created: NLD 2003-01-14
//-----------------------------------------------------------------------------
inline
const T_PointVector& MIL_Limit::GetPointVector() const
{
    return pointVector_;
}


//-----------------------------------------------------------------------------
// Name: MIL_Limit::GetLength
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
inline
MT_Float MIL_Limit::GetLength() const
{
    MT_Float rLength = 0.;
    const MT_Vector2D* pPrevPoint = 0;
    for( CIT_PointVector itPoint = pointVector_.begin(); itPoint != pointVector_.end(); ++itPoint )
    {
        const MT_Vector2D& curPoint = *itPoint;

        if( pPrevPoint )
            rLength += pPrevPoint->Distance( curPoint );
        pPrevPoint = &curPoint;
    }
    return rLength;
}
