// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionMoveUnderground.h"
#include "PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/UndergroundAttribute.h"
#include "Knowledge/DEC_Knowledge_Object.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionMoveUnderground constructor
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
PHY_ActionMoveUnderground::PHY_ActionMoveUnderground( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pDestination )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_( pion.GetRole< PHY_RoleAction_MovingUnderground >() )
{
    if( role_.InitializeUndergroundMoving( pDestination ) )
        Callback( static_cast< unsigned int >( eRunning) );
    else
        Callback( static_cast< unsigned int >( eNotAllowed ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMoveUnderground destructor
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
PHY_ActionMoveUnderground::~PHY_ActionMoveUnderground()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMoveUnderground::Execute
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_ActionMoveUnderground::Execute()
{
    if( role_.Run() )
        Callback( static_cast< unsigned int >( eRunning ) );
    else
        Callback( static_cast< unsigned int >( eFinished ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMoveUnderground::ExecuteSuspended
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_ActionMoveUnderground::ExecuteSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMoveUnderground::StopAction
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
void PHY_ActionMoveUnderground::StopAction()
{
    Callback( static_cast< unsigned int >( eFinished ) );
}
