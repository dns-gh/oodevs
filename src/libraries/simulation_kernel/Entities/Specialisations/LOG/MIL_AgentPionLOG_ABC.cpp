// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AgentPionLOG_ABC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:32 $
// $Revision: 6 $
// $Workfile: MIL_AgentPionLOG_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPionLOG_ABC.h"
#include "MIL_AutomateLOG.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include <xeumeuleu/xml.hpp>


// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis )
    : MIL_AgentPion   ( type, automate, algorithmFactories, xis )
    , pLogisticAction_( new PHY_ActionLogistic< MIL_AgentPionLOG_ABC >( *this ) )
{
    this->RegisterAction( pLogisticAction_ );
    if( !GetAutomate().GetType().IsLogistic() )
        xis.error( "The automata of this pion is not a logistic one." );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name )
    : MIL_AgentPion   ( type, automate, algorithmFactories, name )
    , pLogisticAction_( new PHY_ActionLogistic< MIL_AgentPionLOG_ABC >( *this ) )
{
    this->RegisterAction( pLogisticAction_ );
    assert( automate.GetType().IsLogistic() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories )
    : MIL_AgentPion   ( type, algorithmFactories )
    , pLogisticAction_( new PHY_ActionLogistic< MIL_AgentPionLOG_ABC >( *this ) )
{
    this->RegisterAction( pLogisticAction_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::~MIL_AgentPionLOG_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void MIL_AgentPionLOG_ABC::UpdateLogistic()
{
    const bool bIsDead = IsDead();
    PHY_RoleInterface_Maintenance* roleMaintenance = RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( roleMaintenance )
        roleMaintenance->UpdateLogistic( bIsDead );
    PHY_RoleInterface_Medical* roleMedical = RetrieveRole< PHY_RoleInterface_Medical >();
    if( roleMedical )
        roleMedical->UpdateLogistic( bIsDead );
    PHY_RoleInterface_Supply* roleSupply = RetrieveRole< PHY_RoleInterface_Supply >();
    if( roleSupply )
        roleSupply->UpdateLogistic( bIsDead );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::FindLogisticManager
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_AgentPionLOG_ABC::FindLogisticManager() const
{
    return GetAutomate().FindLogisticManager();
}
