// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_FireResult::GetTarget
// Created: SBO 2005-08-31
// -----------------------------------------------------------------------------
inline
const MOS_Agent_ABC& MOS_FireResult::GetTarget() const
{
    assert( pTarget_ );
    return *pTarget_;
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResult::GetEquipments
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
const MOS_FireResult::T_FireResultEquipments& MOS_FireResult::GetEquipments() const
{
    return equipments_;
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResult::GetHumans
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
const MOS_FireResult::T_FireResultHumans& MOS_FireResult::GetHumans() const
{
    return humans_;
}

// -----------------------------------------------------------------------------
// Name: MOS_FireResult::GetDead
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
inline
uint MOS_FireResult::GetDead() const
{
    return nDead_;
}
