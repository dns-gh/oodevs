//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.inl $
// $Author: Age $
// $Modtime: 25/02/05 9:59 $
// $Revision: 2 $
// $Workfile: DEC_Path.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: DEC_Path::GetState
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
inline
DEC_Path_ABC::E_State DEC_Path::GetState() const
{   
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetComputedWaypoints
// Created: CMA 2012-02-23
// -----------------------------------------------------------------------------
inline
const T_PointVector& DEC_Path::GetComputedWaypoints() const
{
    return computedWaypoints_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::RegisterPathSection
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
void DEC_Path::RegisterPathSection( DEC_PathSection_ABC& section )
{
    pathSections_.push_back( &section );
}
