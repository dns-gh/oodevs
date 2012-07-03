// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetEnemyMaximumCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetEnemyMaximumCost() const
{
    return rEnemyMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetEnemyCostOnContact
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetEnemyCostOnContact() const
{
    return rEnemyCostOnContact_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetEnemyCostAtSecurityRange
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetEnemyCostAtSecurityRange() const
{
    return rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPreferedTerrainCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetPreferedTerrainCost() const
{
    return rPreferedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPreferedTerrain
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const TerrainData& Agent_PathClass::GetPreferedTerrain() const
{
    return preferedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAvoidedTerrainCost_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetAvoidedTerrainCost() const
{
    return rAvoidedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAvoidedTerrain_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const TerrainData& Agent_PathClass::GetAvoidedTerrain() const
{
    return avoidedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAltitudePreference
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetAltitudePreference() const
{
    return rAltitudePreference_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetComfortFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetComfortFuseauDistance() const
{
    return rComfortFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetFuseauCostPerMeterOut() const
{
    return rFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetFuseauCostPerMeterIn
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetFuseauCostPerMeterIn() const
{
    return rFuseauCostPerMeterIn_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetMaximumFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetMaximumFuseauDistance() const
{
    return rMaximumFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetMaximumFuseauDistanceWithAutomata
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetMaximumFuseauDistanceWithAutomata() const
{
    return rMaximumFuseauDistanceWithAutomata_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetAutomateFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetAutomateFuseauCostPerMeterOut() const
{
    return rAutomataFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetMaximumAutomataFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetMaximumAutomataFuseauDistance() const
{
    return rMaximumAutomataFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetDangerDirectionBaseCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetDangerDirectionBaseCost() const
{
    return rDangerDirectionBaseCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetDangerDirectionLinearCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetDangerDirectionLinearCost() const
{
    return rDangerDirectionLinearCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::AvoidObjects
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool Agent_PathClass::AvoidObjects() const
{
    return bAvoidObjects_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::AvoidEnemies
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool Agent_PathClass::AvoidEnemies() const
{
    return rEnemyCostOnContact_ || rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::HandlePopulations
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
bool Agent_PathClass::HandlePopulations() const
{
    return !populationAttitudeCosts_.empty();
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::IsShort
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool Agent_PathClass::IsShort() const
{
    return bShort_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::IsFlying
// Created: LDC 2011-08-09
// -----------------------------------------------------------------------------
inline
bool Agent_PathClass::IsFlying() const
{
    return bFlying_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPopulationSecurityRange
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetPopulationSecurityRange() const
{
    return rPopulationSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetPopulationMaximumCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetPopulationMaximumCost() const
{
    return rPopulationMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetThreshold
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetThreshold() const
{
    return rObstructionThreshold_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathClass::GetCostOutsideOfPopulation
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
inline
double Agent_PathClass::GetCostOutsideOfPopulation() const
{
    return rCostOutsideOfPopulation_;
}
