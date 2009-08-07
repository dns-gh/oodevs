// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionInfluenceInArea.h"
#include "PHY_RoleAction_FolkInfluence.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"

#include "folk/Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionInfluenceInArea constructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionInfluenceInArea::PHY_ActionInfluenceInArea( MIL_AgentPion& pion, const std::string& activity, double influence, const TER_Localisation* pLocalisation )
    : PHY_DecisionCallbackAction_ABC      ( pion )
    , pion_               ( pion )
    , role_               ( pion.GetRole< PHY_RoleAction_FolkInfluence >() )
    , activity_           ( activity )
    , influence_          ( influence )
{    
    // Register( GetRole().InfluenceActivityInArea( diaCall[ 1 ).ToString(), diaCall[ 2 ].ToFloat(), pLocalisation ) );
    const MT_Vector2D&  vPos = pion_.GetRole< PHY_RolePion_Location >().GetPosition();
    if ( pLocalisation )
        pObject_ = role_.InfluenceActivityInArea( vPos, activity_, influence_, *pLocalisation );
    if ( !pObject_ )
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eImpossible ) );
    else
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eRunning ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInfluenceInArea destructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionInfluenceInArea::~PHY_ActionInfluenceInArea()
{
    if ( pObject_ )
        role_.ReleaseInfluence( *pObject_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_ActionInfluenceInArea::Execute
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionInfluenceInArea::Execute()
{
    if ( pObject_ && !pObject_->IsActivated() )
    {
        pObject_->Activate();
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eActivated ) );
    }
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ActionInfluenceInArea::ExecuteSuspended
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionInfluenceInArea::ExecuteSuspended()
{    
    if ( pObject_ )
    {
        pObject_->Deactivate();
        Callback( static_cast< int >( PHY_RoleAction_FolkInfluence::eDeactivated ) );
    }
}
