// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionTriggerActivityInArea.h"
#include "PHY_RoleAction_FolkInfluence.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivityInArea constructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionTriggerActivityInArea::PHY_ActionTriggerActivityInArea( MIL_AgentPion& pion, const std::string& activity, double influence, const TER_Localisation* /*pLocalisation*/ )
    : PHY_DecisionCallbackAction_ABC( pion )
    , pion_     ( pion )
    , role_     ( pion.GetRole< PHY_RoleAction_FolkInfluence >() )
    , activity_ ( activity )
    , influence_( influence )
{
    Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eImpossible ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivityInArea destructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionTriggerActivityInArea::~PHY_ActionTriggerActivityInArea()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivityInArea::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionTriggerActivityInArea::StopAction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivityInArea::Execute
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionTriggerActivityInArea::Execute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivityInArea::ExecuteSuspended
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionTriggerActivityInArea::ExecuteSuspended()
{
    // NOTHING
}
