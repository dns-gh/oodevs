//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path.inl $
// $Author: Age $
// $Modtime: 13/04/05 18:34 $
// $Revision: 3 $
// $Workfile: DEC_Path.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::InsertDecPoints
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void DEC_Population_Path::InsertDecPoints()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::GetChannelers
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
inline
const DEC_Population_Path::T_PopulationPathChannelerVector& DEC_Population_Path::GetChannelers() const
{
    return channelers_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path::NeedRefine
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
inline
bool DEC_Population_Path::NeedRefine() const
{
    return false;
}
