// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetDensity
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationElement_ABC::GetDensity() const
{
    return rDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetAttitude
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude& MIL_PopulationElement_ABC::GetAttitude() const
{
    assert( pAttitude_ );
    return *pAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::Clean()
{
    bAttitudeUpdated_ = false;   
    bHumansUpdated_   = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::HasHumansChanged
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationElement_ABC::HasHumansChanged() const
{
    return bHumansUpdated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::HasAttitudeChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationElement_ABC::HasAttitudeChanged() const
{
    return bAttitudeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetPopulation
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
inline
MIL_Population& MIL_PopulationElement_ABC::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetID
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationElement_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetNbrAliveHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationElement_ABC::GetNbrAliveHumans() const
{
    return rNbrAliveHumans_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::GetNbrDeadHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationElement_ABC::GetNbrDeadHumans() const
{
    return rNbrDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::UpdateDensity
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::UpdateDensity()
{
    const MT_Float rArea = GetLocation().GetArea();
    if( rArea == 0. )
        rDensity_ = 0; // $$$ +infini sauf si aucun humain
    else
        rDensity_ = rNbrAliveHumans_ / rArea;    
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::SetAttitude
// Created: SBO 2005-10-25
// ----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::SetAttitude( const MIL_PopulationAttitude& attitude )
{
    if( pAttitude_ != &attitude )
    {
        bAttitudeUpdated_ = true;
        pAttitude_ = &attitude;
    }
}
// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::SetNbrAliveHumans
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::SetNbrAliveHumans( MT_Float rNbr )
{
    if( rNbrAliveHumans_ == rNbr )
        return;

    rNbrAliveHumans_ = rNbr;
    bHumansUpdated_  = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::SetNbrDeadHumans
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::SetNbrDeadHumans( MT_Float rNbr )
{
    if( rNbrDeadHumans_ == rNbr )
        return;

    rNbrDeadHumans_ = rNbr;
    bHumansUpdated_  = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::PushHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationElement_ABC::PushHumans( MT_Float rNbr )
{
    bHumansUpdated_ = true;
    rNbrAliveHumans_ += rNbr;
    return rNbr;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::PullHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationElement_ABC::PullHumans( MT_Float rNbr )
{
    bHumansUpdated_ = true;
    const MT_Float rNbrTmp = std::min( rNbr, rNbrAliveHumans_ );
    rNbrAliveHumans_ -= rNbrTmp;
    return rNbrTmp;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::IsDead
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationElement_ABC::IsDead() const
{
    return rNbrAliveHumans_ == 0.;
}
