// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionTriggerActivity.h"
#include "PHY_RoleAction_FolkInfluence.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

#include "folk/Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivity constructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionTriggerActivity::PHY_ActionTriggerActivity( MIL_AgentPion& pion, const std::string& activity, double influence )
    : PHY_DecisionCallbackAction_ABC      ( pion )
    , pion_               ( pion )
    , role_               ( pion.GetRole< PHY_RoleAction_FolkInfluence >() )
    , activity_           ( activity )
    , influence_          ( influence )
{
    const double connectivity = 1;
    const MT_Vector2D&  vPos = pion_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    pObject_ = role_.InfluenceActivity( vPos, activity_, influence_, connectivity );
    if ( pObject_ )
    {
        pObject_->TriggerActivity( activity_, influence_ ); // population concerned
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eRunning ) );
    }        
    else
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eImpossible ) );    
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivity destructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionTriggerActivity::~PHY_ActionTriggerActivity()
{
    if ( pObject_ )
        role_.ReleaseInfluence( *pObject_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivity::Execute
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionTriggerActivity::Execute()
{
    if ( pObject_ && !pObject_->IsActivated() )
    {
        pObject_->Activate();
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eActivated ) );
    }
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ActionTriggerActivity::ExecuteSuspended
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionTriggerActivity::ExecuteSuspended()
{
    if ( pObject_ )
    {
        pObject_->Deactivate();
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eDeactivated ) );
    }
}
