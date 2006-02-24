// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetEnemyMaximumCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetEnemyMaximumCost() const
{
    return rEnemyMaximumCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetEnemyCostOnContact
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetEnemyCostOnContact() const
{
    return rEnemyCostOnContact_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetEnemyCostAtSecurityRange
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetEnemyCostAtSecurityRange() const
{
    return rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetPreferedTerrainCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetPreferedTerrainCost() const
{
    return rPreferedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetPreferedTerrain
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const TerrainData& DEC_PathClass::GetPreferedTerrain() const
{
    return preferedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetAvoidedTerrainCost_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetAvoidedTerrainCost() const
{
    return rAvoidedTerrainCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetAvoidedTerrain_
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const TerrainData& DEC_PathClass::GetAvoidedTerrain() const
{
    return avoidedTerrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetAltitudePreference
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetAltitudePreference() const
{
    return rAltitudePreference_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetComfortFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetComfortFuseauDistance() const
{
    return rComfortFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetFuseauCostPerMeterOut() const
{
    return rFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetFuseauCostPerMeterIn
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetFuseauCostPerMeterIn() const
{
    return rFuseauCostPerMeterIn_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetMaximumFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetMaximumFuseauDistance() const
{
    return rMaximumFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetMaximumFuseauDistanceWithAutomata
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetMaximumFuseauDistanceWithAutomata() const
{
    return rMaximumFuseauDistanceWithAutomata_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetAutomateFuseauCostPerMeterOut
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetAutomateFuseauCostPerMeterOut() const
{
    return rAutomataFuseauCostPerMeterOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetMaximumAutomataFuseauDistance
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetMaximumAutomataFuseauDistance() const
{
    return rMaximumAutomataFuseauDistance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetDangerDirectionBaseCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetDangerDirectionBaseCost() const
{
    return rDangerDirectionBaseCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetDangerDirectionLinearCost
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetDangerDirectionLinearCost() const
{
    return rDangerDirectionLinearCost_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::AvoidObjects
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_PathClass::AvoidObjects() const
{
    return bAvoidObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::AvoidEnemies
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_PathClass::AvoidEnemies() const
{
    return rEnemyCostOnContact_ || rEnemyCostAtSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::AvoidPopulations
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
bool DEC_PathClass::AvoidPopulations() const
{
    return !populationAttitudeCosts_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::IsShort
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_PathClass::IsShort() const
{
    return bShort_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetPopulationSecurityRange
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetPopulationSecurityRange() const
{
    return rPopulationSecurityRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathClass::GetPopulationMaximumCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
MT_Float DEC_PathClass::GetPopulationMaximumCost() const
{
    return rPopulationMaximumCost_;
}
