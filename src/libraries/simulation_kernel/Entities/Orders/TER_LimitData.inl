// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: TER_LimitData::GetPoints
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const T_PointVector& TER_LimitData::GetPoints() const
{
    return points_;
}

//-----------------------------------------------------------------------------
// Name: TER_LimitData::GetLength
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
inline
MT_Float TER_LimitData::GetLength() const
{
    MT_Float rLength = 0.;
    const MT_Vector2D* pPrevPoint = 0;
    for( CIT_PointVector itPoint = points_.begin(); itPoint != points_.end(); ++itPoint )
    {
        const MT_Vector2D& curPoint = *itPoint;

        if( pPrevPoint )
            rLength += pPrevPoint->Distance( curPoint );
        pPrevPoint = &curPoint;
    }
    return rLength;
}