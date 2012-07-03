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
#include "MIL_AgentPionLOGMaintenance.h"

#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.h"

#include "simulation_kernel/AlgorithmsFactories.h"

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
// Name: MIL_AgentTypePionLOGMaintenance::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGMaintenance::InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis ) const
{
    return new MIL_AgentPionLOGMaintenance( *this, automate, algorithmFactories, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGMaintenance::InstanciatePion( MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name ) const
{
    return new MIL_AgentPionLOGMaintenance( *this, automate, algorithmFactories, name );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMaintenance::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in AgentFactory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGMaintenance::RegisterRoles( MIL_AgentPion& pion ) const
{
    MIL_AgentTypePion::RegisterRoles( pion );
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
