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
#include "Entities/Agents/RoleExtender_ABC.h"

#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Network/NET_RolePion_Dotations.h"
#include "Adapters/RoleAdapterInterface.h"

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

MIL_AgentPion* MIL_AgentTypePion_Remote::InstanciatePion( MissionController_ABC& controller,
                                                          MIL_Automate& automate,
                                                          xml::xistream& xis ) const
{
    return new MIL_AgentPion_Remote( *this, controller, automate, xis );
}

namespace
{
    template< typename Role >
    void RegisterRoleExt( MIL_AgentPion& pion, sword::RoleExtender_ABC& ext )
    {
        boost::shared_ptr< sword::RoleExtender_ABC::RoleFactory_ABC > fact( ext.GetFactory< Role >() );
        if( fact.get() )
            fact->RegisterRole( pion );
    }
}

void MIL_AgentTypePion_Remote::RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const
{
    pion.RegisterRole( *new PHY_RolePion_UrbanLocation( pion ) );
    pion.RegisterRole( *new network::NET_RolePion_Dotations( pion ) );
    if( ext )
    {
        RegisterRoleExt< PHY_RoleInterface_Location >( pion, *ext );
        RegisterRoleExt< PHY_RoleInterface_Composantes >( pion, *ext );
        RegisterRoleExt< sword::RoleAdapterInterface >( pion, *ext );
    }
}
