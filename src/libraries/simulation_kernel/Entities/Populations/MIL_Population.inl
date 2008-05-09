// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetKnowledge
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
inline
const DEC_PopulationKnowledge& MIL_Population::GetKnowledge() const
{
    assert( pKnowledge_ );
    return *pKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDefaultAttitude
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude& MIL_Population::GetDefaultAttitude() const
{
    assert( pDefaultAttitude_ );
    return *pDefaultAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetID
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
uint MIL_Population::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetType
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
const MIL_PopulationType& MIL_Population::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetOrderManager
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
inline
const MIL_PopulationOrderManager& MIL_Population::GetOrderManager() const
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetOrderManager
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
inline
MIL_PopulationOrderManager& MIL_Population::GetOrderManager()
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetArmy
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
const MIL_Army& MIL_Population::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetPionMaxSpeed
// Created: NLD 2005-10-24
// -----------------------------------------------------------------------------
inline
void MIL_Population::SetPionMaxSpeed( MT_Float rSpeed )
{
    assert( rSpeed >= 0. );
    bPionMaxSpeedOverloaded_ = true;
    rOverloadedPionMaxSpeed_ = rSpeed;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::ResetPionMaxSpeed
// Created: NLD 2005-10-24
// -----------------------------------------------------------------------------
inline
void MIL_Population::ResetPionMaxSpeed()
{
    bPionMaxSpeedOverloaded_ = false;
    rOverloadedPionMaxSpeed_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasDoneMagicMove
// Created: NLD 2005-12-07
// -----------------------------------------------------------------------------
inline
bool MIL_Population::HasDoneMagicMove() const
{
    return bHasDoneMagicMove_;
}
