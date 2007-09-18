// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionUnloadActivity.h"
#include "PHY_RoleAction_FolkInfluence.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"

#include "folk/Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadActivity constructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionUnloadActivity::PHY_ActionUnloadActivity( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC      ( pion, diaCall )
    , pion_               ( pion )
    , role_               ( pion.GetRole< PHY_RoleAction_FolkInfluence >() )
    , diaReturnCode_      ( diaCall[ 0 ] )
    , activity_           ( diaCall[ 1 ].ToString() )
    , influence_          ( diaCall[ 2 ].ToFloat() )
{    
    const MT_Vector2D&  vPos = pion_.GetRole< PHY_RolePion_Location >().GetPosition();
    float connectivity = diaCall.GetParameter( 3 ).ToFloat();
    pObject_ = role_.InfluenceActivity( vPos, activity_, influence_, connectivity );
    if ( pObject_ )
    {
        pObject_->UnloadActivity( activity_, influence_ ); // population concerned
        diaReturnCode_.SetValue( PHY_RoleAction_FolkInfluence::eRunning );
    }        
    else
        diaReturnCode_.SetValue( PHY_RoleAction_FolkInfluence::eImpossible );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadActivity destructor
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
PHY_ActionUnloadActivity::~PHY_ActionUnloadActivity()
{
    if ( pObject_ )
        role_.ReleaseInfluence( *pObject_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadActivity::Execute
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionUnloadActivity::Execute()
{
    if ( pObject_ && !pObject_->IsActivated() )
    {
        pObject_->Activate();
        diaReturnCode_.SetValue( PHY_RoleAction_FolkInfluence::eActivated );
    }
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadActivity::ExecuteSuspended
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
void PHY_ActionUnloadActivity::ExecuteSuspended()
{
    if ( pObject_ )
    {
        pObject_->Deactivate();
        diaReturnCode_.SetValue( PHY_RoleAction_FolkInfluence::eDeactivated );
    }
}
