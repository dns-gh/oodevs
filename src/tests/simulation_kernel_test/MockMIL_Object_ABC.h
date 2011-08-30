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
    MOCK_METHOD( WriteODB, 1 );
    MOCK_METHOD( PreprocessAgent, 1 );
    MOCK_METHOD( ProcessAgentEntering, 1 );
    MOCK_METHOD( ProcessAgentExiting, 1 );
    MOCK_METHOD( ProcessAgentMovingInside, 1 );
    MOCK_METHOD( ProcessAgentInside, 1 );
    MOCK_METHOD( PreprocessPopulation, 1 );
    MOCK_METHOD( ProcessPopulationInside, 1 );
    MOCK_METHOD( ApplyIndirectFire, 3 );
    MOCK_METHOD( ApplyDirectFire, 0 );
    MOCK_METHOD( ApplyDestruction, 2 );
    MOCK_METHOD( IsUniversal, 0 );
    MOCK_METHOD_EXT( CreateKnowledge, 1, boost::shared_ptr< DEC_Knowledge_Object >( const MIL_Army_ABC& ) , CreateKnowledgeArmy );
    MOCK_METHOD_EXT( CreateKnowledge, 1, boost::shared_ptr< DEC_Knowledge_Object >( const MIL_KnowledgeGroup& ) , CreateKnowledgeKnowledgeGroup );
    MOCK_CONST_METHOD_EXT( operator(), 0, const MIL_ObjectManipulator_ABC&(), OperatorConst );
    MOCK_NON_CONST_METHOD_EXT( operator(), 0, MIL_ObjectManipulator_ABC&(), OperatorNonConst );
    MOCK_METHOD( OnUpdate, 1 );
    MOCK_METHOD( SendCreation, 0 );
    MOCK_METHOD( SendDestruction, 0 );
    MOCK_METHOD( SendFullState, 0 );
    MOCK_METHOD( GetID, 0 );
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Instanciate, 1 );
    MOCK_METHOD( Finalize, 0 );
    MOCK_METHOD( ApplyStructuralState, 1 );
    MOCK_METHOD_EXT( Register, 1, void (ObjectCapacity_ABC *), RegisterCapacity );
    MOCK_METHOD_EXT( Register, 1, void (ObjectAttribute_ABC *), RegisterAttribute );
    MOCK_METHOD_EXT( Register, 1, void (MIL_InteractiveContainer_ABC *), RegisterContainer );
    MOCK_METHOD_EXT( Register, 1, void (MIL_StructuralStateNotifier_ABC &), RegisterNotifier );
};

#endif // __MockMIL_Object_ABC_h_
