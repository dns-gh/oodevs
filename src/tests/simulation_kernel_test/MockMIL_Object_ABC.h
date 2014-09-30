// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockMIL_Object_ABC_h_
#define __MockMIL_Object_ABC_h_

#include "protocol/Protocol.h"
#include "Entities/Objects/MIL_Object_ABC.h"

// =============================================================================
/** @class  MockMIL_Object_ABC
    @brief  MockMIL_Object_ABC
*/
// Created: LDC 2010-04-29
// =============================================================================
MOCK_BASE_CLASS( MockMIL_Object_ABC, MIL_Object_ABC )
{
    MockMIL_Object_ABC()
    {}
    MockMIL_Object_ABC( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned int forcedId )
        : MIL_Object_ABC( army, type, forcedId )
    {}

    MOCK_METHOD( WriteODB, 1 );
    MOCK_METHOD( PreprocessAgent, 1 );
    MOCK_METHOD( ProcessAgentEntering, 1 );
    MOCK_METHOD( ProcessAgentExiting, 1 );
    MOCK_METHOD( ProcessAgentMovingInside, 3 );
    MOCK_METHOD( ProcessAgentInside, 1 );
    MOCK_METHOD( ProcessPopulationInside, 1 );
    MOCK_METHOD( ApplyIndirectFire, 3 );
    MOCK_METHOD( ApplyDirectFire, 0 );
    MOCK_METHOD( ApplyDestruction, 2 );
    MOCK_METHOD( IsUniversal, 0 );
    MOCK_METHOD( CanBeSeen, 0 );
    MOCK_METHOD( CreateKnowledge, 1, boost::shared_ptr< DEC_Knowledge_Object >( const MIL_KnowledgeGroup& ) , CreateKnowledgeKnowledgeGroup );
    MOCK_METHOD( CreateKnowledge, 2, boost::shared_ptr< DEC_Knowledge_Object >( const MIL_KnowledgeGroup&, const DEC_Knowledge_Object& ) , CreateKnowledgeArmyKnowledgeGroup );
    MOCK_CONST_METHOD( operator(), 0, const MIL_ObjectManipulator_ABC&(), OperatorConst );
    MOCK_NON_CONST_METHOD( operator(), 0, MIL_ObjectManipulator_ABC&(), OperatorNonConst );
    MOCK_METHOD( OnUpdate, 1 );
    MOCK_METHOD( SendCreation, 0 );
    MOCK_METHOD( SendDestruction, 0 );
    MOCK_METHOD( SendFullState, 0 );
    MOCK_METHOD( GetID, 0 );
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( SetName, 1 );
    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Instanciate, 1 );
    MOCK_METHOD( Finalize, 0 );
    MOCK_METHOD( ApplyStructuralState, 1 );
    MOCK_METHOD( SetExtensions, 1 );
    MOCK_METHOD( Register, 1, void (ObjectCapacity_ABC *), RegisterCapacity );
    MOCK_METHOD( Register, 1, void (ObjectAttribute_ABC *), RegisterAttribute );
    MOCK_METHOD( Register, 1, void (MIL_InteractiveContainer_ABC *), RegisterContainer );
    MOCK_METHOD( Register, 1, void (MIL_StructuralStateNotifier_ABC &), RegisterNotifier );
    MOCK_METHOD_EXT( Unregister, 1, void (MIL_StructuralStateNotifier_ABC &), UnregisterNotifier );
};

#endif // __MockMIL_Object_ABC_h_
