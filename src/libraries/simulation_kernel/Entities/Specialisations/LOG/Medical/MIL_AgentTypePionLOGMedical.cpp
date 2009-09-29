// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Medical/MIL_AgentTypePionLOGMedical.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGMedical.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOGMedical.h"
#include "MIL_AgentPionLOGMedical.h"

#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"
#include "simulation_kernel/AlgorithmsFactories.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMedical::MIL_AgentTypePionLOGMedical( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGMedical::~MIL_AgentTypePionLOGMedical()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGMedical::InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const
{
    return new MIL_AgentPionLOGMedical( *this, nID, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePionLOGMedical::InstanciatePion( uint nID, MIL_Automate& automate ) const
{
    return new MIL_AgentPionLOGMedical( *this, nID, automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in AgentFactory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGMedical::RegisterRoles( MIL_AgentPion& pion, AlgorithmsFactories& algorithmsFactories ) const
{
    MIL_AgentTypePion::RegisterRoles( pion, algorithmsFactories  );
    pion.RegisterRole< PHY_RolePionLOG_Medical >( static_cast< MIL_AgentPionLOG_ABC& >( pion ), *algorithmsFactories.onComponentFunctorComputerFactory_, *algorithmsFactories.onComponentLendedFunctorComputerFactory_ );//@TODO remove cast when AgentPionLOG will be deleted   
}


// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionLOGMedical::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGMedical( strName, xis );
}
