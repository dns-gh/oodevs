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
// Name: MIL_PopulationElement_ABC::GetNbrHumans
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationElement_ABC::GetNbrHumans() const
{
    return rNbrDeadHumans_ + rNbrAliveHumans_;
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
// Name: MIL_PopulationElement_ABC::PushHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::PushHumans( const T_Humans& humans )
{
    if( humans.first == 0. && humans.second == 0. )
        return;

    bHumansUpdated_ = true;
    rNbrAliveHumans_ += humans.first;
    rNbrDeadHumans_  += humans.second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::PullHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
MIL_PopulationElement_ABC::T_Humans MIL_PopulationElement_ABC::PullHumans( MT_Float rNbr )
{
    const MT_Float rNbrAlive = std::min( rNbr, rNbrAliveHumans_ );
    rNbrAliveHumans_ -= rNbrAlive;

    const MT_Float rNbrDead = std::min( rNbr - rNbrAlive, rNbrDeadHumans_ );
    rNbrDeadHumans_ -= rNbrDead;

    if( rNbrAlive > 0. || rNbrDead > 0. )
        bHumansUpdated_ = true;

    return T_Humans( rNbrAlive, rNbrDead );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::KillAllHumans
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::KillAllHumans()
{
    if( rNbrAliveHumans_ == 0. )
        return;

    rNbrDeadHumans_ += rNbrAliveHumans_;
    rNbrAliveHumans_ = 0.;
    bHumansUpdated_  = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::Exterminate
// Created: SBO 2005-12-22
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationElement_ABC::Exterminate( MT_Float rSurface )
{
    assert( rDensity_ != 0 );
    MT_Float rKills = rSurface * rDensity_;

    if( rKills > rNbrAliveHumans_ )
        rKills = rNbrAliveHumans_;

    rNbrDeadHumans_  += rKills;
    rNbrAliveHumans_ -= rKills;
    bHumansUpdated_   = true;
    return rKills / rDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::Kill
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationElement_ABC::Kill( uint count )
{
    uint kills = count;
    if( count > rNbrAliveHumans_ )
    {
        kills = uint( ceil( rNbrAliveHumans_ ) );
        rNbrDeadHumans_ += rNbrAliveHumans_;
        rNbrAliveHumans_ = 0.f;
    }
    else
    {
        rNbrAliveHumans_ -= kills;
        rNbrDeadHumans_  += kills;
    }
    bHumansUpdated_ = true;
    return kills;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::Resurrect
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationElement_ABC::Resurrect( uint count )
{
    uint resurrections = count;
    if( count > rNbrDeadHumans_ )
    {
        resurrections = uint( ceil( rNbrDeadHumans_ ) );
        rNbrAliveHumans_ += rNbrDeadHumans_;
        rNbrDeadHumans_ = 0.f;
    }
    else
    {
        rNbrAliveHumans_ += resurrections;
        rNbrDeadHumans_  -= resurrections;
    }
    bHumansUpdated_ = true;
    return resurrections;
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

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationElement_ABC::IsInZone( const TER_Localisation& loc ) const
{
    return GetLocation().IsIntersecting( loc );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationElement_ABC::ClearCollisions
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
inline
void MIL_PopulationElement_ABC::ClearCollisions()
{
    collidingAgents_.clear();
}
