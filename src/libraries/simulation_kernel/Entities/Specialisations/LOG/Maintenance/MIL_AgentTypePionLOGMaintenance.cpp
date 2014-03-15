// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Maintenance/MIL_AgentTypePionLOGMaintenance.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGMaintenance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOGMaintenance.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"

#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.h"


// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMaintenance::MIL_AgentTypePionLOGMaintenance( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, strType, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMaintenance::~MIL_AgentTypePionLOGMaintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in AgentFactory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGMaintenance::RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const
{
    MIL_AgentTypePion::RegisterRoles( pion, ext );
    pion.RegisterRole( *new PHY_RolePionLOG_Maintenance( static_cast< MIL_AgentPionLOG_ABC& >( pion ) ) );//@TODO remove cast when AgentPionLOG will be deleted
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionLOGMaintenance::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGMaintenance( strName, strType, xis );
}
