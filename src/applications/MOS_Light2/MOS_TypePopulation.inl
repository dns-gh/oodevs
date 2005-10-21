// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation::GetName
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
const std::string& MOS_TypePopulation::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation::GetID
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
uint MOS_TypePopulation::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation::GetModel
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
inline
const MOS_AgentModel& MOS_TypePopulation::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation::GetConcentrationDensity
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float MOS_TypePopulation::GetConcentrationDensity() const
{
    return rConcentrationDensity_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation::GetDefaultFlowDensity
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float MOS_TypePopulation::GetDefaultFlowDensity() const
{
    return rDefaultFlowDensity_;
}

// -----------------------------------------------------------------------------
// Name: MOS_TypePopulation::GetMaxSpeed
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
inline
MT_Float MOS_TypePopulation::GetMaxSpeed() const
{
    return rMaxSpeed_;
}
