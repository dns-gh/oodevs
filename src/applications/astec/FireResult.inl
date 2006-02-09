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
// Name: FireResult::GetTarget
// Created: SBO 2005-08-31
// -----------------------------------------------------------------------------
inline
const Agent_ABC& FireResult::GetTarget() const
{
    assert( pTarget_ );
    return *pTarget_;
}

// -----------------------------------------------------------------------------
// Name: FireResult::GetEquipments
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
const FireResult::T_FireResultEquipments& FireResult::GetEquipments() const
{
    return equipments_;
}

// -----------------------------------------------------------------------------
// Name: FireResult::GetHumans
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
const FireResult::T_FireResultHumans& FireResult::GetHumans() const
{
    return humans_;
}

// -----------------------------------------------------------------------------
// Name: FireResult::GetDead
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
inline
uint FireResult::GetDead() const
{
    return nDead_;
}
