// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RoleHLA_Communications.cpp $
// $Author: Age $
// $Modtime: 2/12/04 11:46 $
// $Revision: 5 $
// $Workfile: PHY_RoleHLA_Communications.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Communications.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Communications constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Communications::PHY_RoleHLA_Communications( MT_RoleContainer& roleContainer )
    : PHY_RoleInterface_Communications( roleContainer )
    , bJammed_( false ) 
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
// Name: PHY_RoleHLA_Communications::CanCommunicate
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Communications::CanCommunicate() const
{
    return ! bJammed_;
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
// Name: PHY_RoleHLA_Communications::ChangeStatus
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Communications::ChangeStatus( const std::vector< std::string >& statuses )
{
    bJammed_ = std::find( statuses.begin(), statuses.end(), "brouille" ) != statuses.end();
}
