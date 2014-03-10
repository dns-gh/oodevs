// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Supply/MIL_AgentTypePionLOGSupply.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:54 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGSupply.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOGSupply.h"
#include "MIL_AgentPionLOGSupply.h"

#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Supply.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGSupply::MIL_AgentTypePionLOGSupply( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, strType, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGSupply::~MIL_AgentTypePionLOGSupply()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGSupply::InstanciatePion( const AlgorithmsFactories& algorithmFactories,
                                                            MissionController_ABC& controller,
                                                            MIL_Automate& automate,
                                                            xml::xistream& xis ) const
{
    return new MIL_AgentPionLOGSupply( *this, algorithmFactories, controller, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGSupply::InstanciatePion( const AlgorithmsFactories& algorithmFactories,
                                                            MissionController_ABC& controller,
                                                            MIL_Automate& automate,
                                                            const std::string& name ) const
{
    return new MIL_AgentPionLOGSupply( *this, algorithmFactories, controller,
            automate, *MakeAgentPionXml( name, automate ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGSupply::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in factory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGSupply::RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const
{
    MIL_AgentTypePion::RegisterRoles( pion, ext );
    pion.RegisterRole( *new PHY_RolePionLOG_Supply( static_cast< MIL_AgentPionLOG_ABC& >( pion ) ) );//@TODO remove cast when AgentPionLOG will be deleted
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionLOGSupply::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGSupply( strName, strType, xis );
}
