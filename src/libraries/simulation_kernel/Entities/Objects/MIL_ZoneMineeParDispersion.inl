//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ZoneMineeParDispersion.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_ZoneMineeParDispersion.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneMineeParDispersion::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_ZoneMineeParDispersion( type, nID, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::GetMinesActivityTime
// Created: NLD 2007-02-07
// -----------------------------------------------------------------------------
inline
uint MIL_ZoneMineeParDispersion::GetMinesActivityTime() const
{
    return nMinesActivityTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::GetMinesDensity
// Created: NLD 2007-02-07
// -----------------------------------------------------------------------------
inline
MT_Float MIL_ZoneMineeParDispersion::GetMinesDensity() const
{
    return rMinesDensity_;
}