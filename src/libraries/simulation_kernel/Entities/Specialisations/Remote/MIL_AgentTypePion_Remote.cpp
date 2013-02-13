// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePion_Remote.h"
#include "MIL_AgentPion_Remote.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"

MIL_AgentTypePion_Remote::MIL_AgentTypePion_Remote( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePion( strName, strType, xis )
{
    // NOTHING
}

MIL_AgentTypePion_Remote::~MIL_AgentTypePion_Remote()
{
    // NOTHING
}

const MIL_AgentTypePion* MIL_AgentTypePion_Remote::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePion_Remote( strName, strType, xis );
}

MIL_AgentPion* MIL_AgentTypePion_Remote::InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis, const MIL_Config& config ) const
{
    MIL_AgentPion* retval = new MIL_AgentPion_Remote( *this, automate, algorithmFactories, xis, config );
    return retval;
}

MIL_AgentPion* MIL_AgentTypePion_Remote::InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name ) const
{
    MIL_AgentPion* retval = new MIL_AgentPion_Remote( *this, automate, algorithmFactories, name );
    return retval;
}

void MIL_AgentTypePion_Remote::RegisterRoles( MIL_AgentPion& pion, RoleExtender_ABC* ) const
{
    pion.RegisterRole( *new PHY_RolePion_UrbanLocation( pion ) );
}
