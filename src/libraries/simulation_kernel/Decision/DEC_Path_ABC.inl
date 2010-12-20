//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_ABC.inl $
// $Author: Age $
// $Modtime: 25/02/05 9:59 $
// $Revision: 2 $
// $Workfile: DEC_Path_ABC.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetState
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------
inline
DEC_Path_ABC::E_State DEC_Path_ABC::GetState() const
{   
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::GetID
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
inline
unsigned int DEC_Path_ABC::GetID() const
{
    return nID_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::RegisterPathSection
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
void DEC_Path_ABC::RegisterPathSection( DEC_PathSection_ABC& section )
{
    pathSections_.push_back( &section );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::operator==
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
bool DEC_Path_ABC::operator==( const DEC_Path_ABC& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_ABC::operator!=
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
bool DEC_Path_ABC::operator!=( const DEC_Path_ABC& rhs ) const
{
    return nID_ != rhs.nID_;
}
