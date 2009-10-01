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

#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.h"

#include "simulation_kernel/AlgorithmsFactories.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2 constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGTC2::MIL_AgentTypePionLOGTC2( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, xis )
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
MIL_AgentPion* MIL_AgentTypePionLOGTC2::InstanciatePion( uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis ) const
{
    return new MIL_AgentPionLOGTC2( *this, nID, automate, algorithmFactories, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGTC2::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGTC2::InstanciatePion( uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories ) const
{
    return new MIL_AgentPionLOGTC2( *this, nID, automate, algorithmFactories );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in AgentFactory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGTC2::RegisterRoles( MIL_AgentPion& pion ) const
{
    MIL_AgentTypePion::RegisterRoles( pion );
    pion.RegisterRole< PHY_RolePionLOG_Maintenance >( static_cast< MIL_AgentPionLOG_ABC& >( pion ) ); //@TODO remove cast when AgentPionLOG will be deleted 
    pion.RegisterRole< PHY_RolePionLOG_Medical     >( static_cast< MIL_AgentPionLOG_ABC& >( pion ) );  
    pion.RegisterRole< PHY_RolePionLOG_Supply      >( static_cast< MIL_AgentPionLOG_ABC& >( pion ) );  
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionLOGTC2::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGTC2( strName, xis );
}
