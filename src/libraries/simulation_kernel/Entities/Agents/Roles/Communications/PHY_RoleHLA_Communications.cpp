// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Communications.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RoleHLA_Communications )

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Communications::PHY_RoleHLA_Communications()
    : bJammed_( false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Communications::~PHY_RoleHLA_Communications()
{
    //NOTHING
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::UpdateKnowledgesFromObjectPerception
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::UpdateKnowledgesFromObjectCollision
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::UpdateKnowledgesFromObjectCollision( const DEC_Knowledge_ObjectCollision& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::Jam
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::Jam( const MIL_Object_ABC& /*jammer*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::Unjam
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::Unjam( const MIL_Object_ABC& /*jammer*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::ActivateBlackout
// Created: AHC 2009-08-18
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::ActivateBlackout( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::ActivatePartialBlackout
// Created: HBD 2010-06-16
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::ActivatePartialBlackout()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::DeactivateBlackout
// Created: AHC 2009-08-18
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::DeactivateBlackout( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::ModifySpeed
// Created: AHC 2009-08-18
// -----------------------------------------------------------------------------
MT_Float PHY_RoleHLA_Communications::ModifySpeed            ( MT_Float rSpeed    ) const
{
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::ChangeStatus
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::ChangeStatus( const std::vector< std::string >& statuses )
{//@TODO MGD USE NOTIFICATION
    bJammed_ = std::find( statuses.begin(), statuses.end(), "brouille" ) != statuses.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::CanEmit
// Created: HBD 2010-06-18
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Communications::CanEmit() const
{
   return ! bJammed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications::CanReceive
// Created: HBD 2010-06-18
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Communications::CanReceive() const
{
   return ! bJammed_;
}
