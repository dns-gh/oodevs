// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleHLA_Population.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleHLA_Population, "PHY_RoleHLA_Population" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Population constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Population::PHY_RoleHLA_Population()
    : bInvulnerable_              ( false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Population destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
PHY_RoleHLA_Population::~PHY_RoleHLA_Population()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Population::IsInvulnerable
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
bool PHY_RoleHLA_Population::IsInvulnerable() const
{
    return bInvulnerable_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleHLA_Population::ChangeStatus
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void PHY_RoleHLA_Population::ChangeStatus( const std::vector< std::string >& statuses )
{
    bInvulnerable_ = std::find( statuses.begin(), statuses.end(), "invulnerable_population" ) != statuses.end();
}
