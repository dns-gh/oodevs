// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Convoy/MIL_AgentTypePionLOGConvoy.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGConvoy.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOGConvoy.h"
#include "MIL_AgentPionLOGConvoy.h"

#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOGConvoy_Supply.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGConvoy::MIL_AgentTypePionLOGConvoy( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, strType, xis )
{
    // $$$ ?? Checker que le type ne contient aucun equipement ?
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGConvoy::~MIL_AgentTypePionLOGConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGConvoy::InstanciatePion( MissionController_ABC& controller,
                                                            MIL_Automate& automate,
                                                            xml::xistream& xis ) const
{
    return new MIL_AgentPionLOGConvoy( *this, controller, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in AgentFactory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGConvoy::RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const
{
    MIL_AgentTypePion::RegisterRoles( pion, ext );
    pion.RegisterRole( *new PHY_RolePionLOGConvoy_Supply( pion ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionLOGConvoy::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGConvoy( strName, strType, xis );
}
