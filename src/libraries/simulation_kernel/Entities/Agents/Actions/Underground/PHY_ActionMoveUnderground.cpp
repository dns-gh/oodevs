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
#include "Entities/Objects/UndergroundCapacity.h"
#include "Knowledge/DEC_Knowledge_Object.h"

namespace
{
    MIL_Object_ABC* IsObjectValid( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
    {
        if( pKnowledge && pKnowledge->IsValid() )
            if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
                if( pObject->Retrieve< UndergroundCapacity >() != 0 )
                    return pObject;
        return 0;
    }

    bool AreObjectsOfSameType( boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge1, boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge2 )
    {
        MIL_Object_ABC* object1 = IsObjectValid( pKnowledge1 );
        MIL_Object_ABC* object2 = IsObjectValid( pKnowledge2 );
        if( object1 && object2 )
            return object1->GetType().GetID() == object2->GetType().GetID();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMoveUnderground constructor
// Created: JSR 2011-06-08
// -----------------------------------------------------------------------------
PHY_ActionMoveUnderground::PHY_ActionMoveUnderground( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pFirstObject, boost::shared_ptr< DEC_Knowledge_Object > pSecondObject )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_( pion.GetRole< PHY_RoleAction_MovingUnderground >() )
{
    if( AreObjectsOfSameType( pFirstObject, pSecondObject ) )
    {
        Callback( static_cast< unsigned int >( eRunning) );
        role_.InitializeUndergroundMoving( *pFirstObject->GetObjectKnown(), *pSecondObject->GetObjectKnown() );
    }
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
    role_.GetOutFromUndergroundNetwork();
}
