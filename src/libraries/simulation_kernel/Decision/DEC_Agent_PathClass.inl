// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetEnemyMaximumCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetEnemyMaximumCost() const
{
    return rEnemyMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetEnemyCostOnContact
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetEnemyCostOnContact() const
{
    return rEnemyCostOnContact_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetEnemyCostAtSecurityRange
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetEnemyCostAtSecurityRange() const
{
    return rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPreferedTerrainCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetPreferedTerrainCost() const
{
    return rPreferedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPreferedTerrain
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const TerrainData& DEC_Agent_PathClass::GetPreferedTerrain() const
{
    return preferedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAvoidedTerrainCost_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetAvoidedTerrainCost() const
{
    return rAvoidedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAvoidedTerrain_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const TerrainData& DEC_Agent_PathClass::GetAvoidedTerrain() const
{
    return avoidedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAltitudePreference
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetAltitudePreference() const
{
    return rAltitudePreference_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetComfortFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetComfortFuseauDistance() const
{
    return rComfortFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetFuseauCostPerMeterOut() const
{
    return rFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetFuseauCostPerMeterIn
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetFuseauCostPerMeterIn() const
{
    return rFuseauCostPerMeterIn_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetMaximumFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetMaximumFuseauDistance() const
{
    return rMaximumFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetMaximumFuseauDistanceWithAutomata
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetMaximumFuseauDistanceWithAutomata() const
{
    return rMaximumFuseauDistanceWithAutomata_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetAutomateFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetAutomateFuseauCostPerMeterOut() const
{
    return rAutomataFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetMaximumAutomataFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetMaximumAutomataFuseauDistance() const
{
    return rMaximumAutomataFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetDangerDirectionBaseCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetDangerDirectionBaseCost() const
{
    return rDangerDirectionBaseCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetDangerDirectionLinearCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetDangerDirectionLinearCost() const
{
    return rDangerDirectionLinearCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::AvoidObjects
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_PathClass::AvoidObjects() const
{
    return bAvoidObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::AvoidEnemies
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_PathClass::AvoidEnemies() const
{
    return rEnemyCostOnContact_ || rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::HandlePopulations
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_PathClass::HandlePopulations() const
{
    return !populationAttitudeCosts_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::IsShort
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_PathClass::IsShort() const
{
    return bShort_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::IsFlying
// Created: LDC 2011-08-09
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_PathClass::IsFlying() const
{
    return bFlying_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPopulationSecurityRange
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetPopulationSecurityRange() const
{
    return rPopulationSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetPopulationMaximumCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetPopulationMaximumCost() const
{
    return rPopulationMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetThreshold
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetThreshold() const
{
    return rObstructionThreshold_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathClass::GetCostOutsideOfPopulation
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathClass::GetCostOutsideOfPopulation() const
{
    return rCostOutsideOfPopulation_;
}
