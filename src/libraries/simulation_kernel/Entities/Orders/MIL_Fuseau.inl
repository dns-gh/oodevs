//*****************************************************************************
//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/MIL_Fuseau.inl $
// $Author: Nld $
// $Modtime: 21/05/04 11:53 $
// $Revision: 1 $
// $Workfile: MIL_Fuseau.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetLeftLimit
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
inline
const MIL_Limit* MIL_Fuseau::GetLeftLimit() const
{
    return pLeftLimit_;
}


//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetRightLimit
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
inline
const MIL_Limit* MIL_Fuseau::GetRightLimit() const
{
    return pRightLimit_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::GetGlobalDirection
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
inline
const MT_Line& MIL_Fuseau::GetGlobalDirection() const
{
    return globalDirectionLine_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::IsNull
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
inline
bool MIL_Fuseau::IsNull() const
{
    return !pLeftLimit_ || !pRightLimit_;
}


//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::IsInside
// Created: NLD 2003-07-27
//-----------------------------------------------------------------------------
inline
bool MIL_Fuseau::IsInside( const MT_Vector2D& vPos  ) const
{
    return TER_Polygon::IsInside( vPos, 0.1 ); //$$$
}


//-----------------------------------------------------------------------------
// Name: MIL_Fuseau::IntersectWithBorder
// Created: NLD 2003-07-27
//-----------------------------------------------------------------------------
inline
bool MIL_Fuseau::IntersectWithBorder( const MT_Line& line ) const
{
    return TER_Polygon::IntersectWithBorder( line, 0.1 ); //$$$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Fuseau::IntersectWithBorder
// Created: NLD 2003-10-15
// -----------------------------------------------------------------------------
inline
bool MIL_Fuseau::IntersectWithBorder( const MT_Line& line,T_PointSet& res ) const
{
    return TER_Polygon::IntersectWithBorder( line, res, 0.1 ); //$$$$
}

