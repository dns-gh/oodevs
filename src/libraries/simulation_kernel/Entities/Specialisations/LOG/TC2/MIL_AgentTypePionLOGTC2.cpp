// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Tc2/MIL_AgentTypePionLOGTC2.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGTC2.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOGTC2.h"
#include "MIL_AgentPionLOGTC2.h"

#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Supply.h"

#include "simulation_kernel/AlgorithmsFactories.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2 constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGTC2::MIL_AgentTypePionLOGTC2( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, strType, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2 destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGTC2::~MIL_AgentTypePionLOGTC2()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGTC2::InstanciatePion( const AlgorithmsFactories& algorithmFactories,
                                                         MissionController_ABC& controller,
                                                         MIL_Automate& automate,
                                                         xml::xistream& xis ) const
{
    return new MIL_AgentPionLOGTC2( *this, algorithmFactories, controller, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in AgentFactory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGTC2::RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const
{
    MIL_AgentTypePion::RegisterRoles( pion, ext );
    pion.RegisterRole( *new PHY_RolePionLOG_Maintenance( static_cast< MIL_AgentPionLOG_ABC& >( pion ) ) ); //@TODO remove cast when AgentPionLOG will be deleted
    pion.RegisterRole( *new PHY_RolePionLOG_Medical    ( static_cast< MIL_AgentPionLOG_ABC& >( pion ) ) );
    pion.RegisterRole( *new PHY_RolePionLOG_Supply     ( static_cast< MIL_AgentPionLOG_ABC& >( pion ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionLOGTC2::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGTC2( strName, strType, xis );
}
