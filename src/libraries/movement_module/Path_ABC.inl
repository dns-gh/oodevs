//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/Path_ABC.inl $
// $Author: Age $
// $Modtime: 25/02/05 9:59 $
// $Revision: 2 $
// $Workfile: Path_ABC.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: Path_ABC::GetState
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
inline
Path_ABC::E_State Path_ABC::GetState() const
{   
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetComputedWaypoints
// Created: CMA 2012-02-23
// -----------------------------------------------------------------------------
inline
const T_PointVector& Path_ABC::GetComputedWaypoints() const
{
    return computedWaypoints_;
}
// -----------------------------------------------------------------------------
// Name: Path_ABC::GetID
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
inline
unsigned int Path_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::GetLastWaypoint
// Created: CMA 2012-05-21
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& Path_ABC::GetLastWaypoint() const
{
    return lastWaypoint_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Path_ABC::RegisterPathSection
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
void Path_ABC::RegisterPathSection( PathSection_ABC& section )
{
    pathSections_.push_back( &section );
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::operator==
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
bool Path_ABC::operator==( const Path_ABC& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: Path_ABC::operator!=
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
bool Path_ABC::operator!=( const Path_ABC& rhs ) const
{
    return nID_ != rhs.nID_;
}
