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
// Name: MIL_PopulationConcentration::GetPosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PopulationConcentration::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetAttitude
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude& MIL_PopulationConcentration::GetAttitude() const
{
    assert( pAttitude_ );
    return *pAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationConcentration::Clean()
{
    bHumansUpdated_   = false;
    bAttitudeUpdated_ = false;   
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::HasChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationConcentration::HasChanged() const
{
    return bHumansUpdated_ || bAttitudeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::RegisterPushingFlow
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
void MIL_PopulationConcentration::RegisterPushingFlow( MIL_PopulationFlow& flow )
{
    bool bOut = pushingFlows_.insert( &flow ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::UnregisterPushingFlow
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
void MIL_PopulationConcentration::UnregisterPushingFlow( MIL_PopulationFlow& flow )
{
    int nOut = pushingFlows_.erase( &flow );
    assert( nOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
const TER_Localisation& MIL_PopulationConcentration::GetLocation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetPopulation
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
inline
MIL_Population& MIL_PopulationConcentration::GetPopulation() const
{
    return population_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetID
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationConcentration::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetNbrAliveHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationConcentration::GetNbrAliveHumans() const
{
    return (uint)rNbrAliveHumans_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetNbrDeadHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationConcentration::GetNbrDeadHumans() const
{
    return (uint)rNbrDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::IsValid
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationConcentration::IsValid() const
{
    return rNbrAliveHumans_ > 0. || !pushingFlows_.empty();
}
