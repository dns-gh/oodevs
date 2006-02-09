// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: TypePopulation::GetName
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
const std::string& TypePopulation::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: TypePopulation::GetID
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
uint TypePopulation::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: TypePopulation::GetModel
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
const AgentModel& TypePopulation::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: TypePopulation::GetConcentrationDensity
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float TypePopulation::GetConcentrationDensity() const
{
    return rConcentrationDensity_;
}

// -----------------------------------------------------------------------------
// Name: TypePopulation::GetDefaultFlowDensity
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float TypePopulation::GetDefaultFlowDensity() const
{
    return rDefaultFlowDensity_;
}

// -----------------------------------------------------------------------------
// Name: TypePopulation::GetMaxSpeed
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float TypePopulation::GetMaxSpeed() const
{
    return rMaxSpeed_;
}
