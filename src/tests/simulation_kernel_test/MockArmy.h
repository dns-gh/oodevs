// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockArmy_h_
#define __MockArmy_h_

#include "simulation_kernel/Entities/MIL_Army_ABC.h"

// =============================================================================
/** @class  MockArmy
    @brief  MockArmy
*/
// Created: JCR 2008-09-01
// =============================================================================
MOCK_BASE_CLASS( MockArmy, MIL_Army_ABC )
{
    MOCK_METHOD( WriteODB, 1 )
    MOCK_METHOD( WriteKnowledges, 1 )
    MOCK_METHOD( WriteDiplomacyODB, 1 )

    MOCK_METHOD( RegisterFormation, 1 )
    MOCK_METHOD( UnregisterFormation, 1 )
    MOCK_METHOD( RegisterObject, 1 )
    MOCK_METHOD( UnregisterObject, 1 )
    MOCK_METHOD( RegisterPopulation, 1 )
    MOCK_METHOD( UnregisterPopulation, 1 )
    MOCK_METHOD( RegisterInhabitant, 1 )
    MOCK_METHOD( UnregisterInhabitant, 1 )
    MOCK_METHOD( Finalize, 0 )

    MOCK_METHOD( UpdateKnowledges, 1 )
    MOCK_METHOD( CleanKnowledges, 0 )
    MOCK_METHOD( CleanDeletedAgentKnowledges, 0 )

    MOCK_METHOD( SendCreation, 0 )
    MOCK_METHOD( SendFullState, 0 )
    MOCK_METHOD( SendKnowledge, 0 )

    MOCK_METHOD( IsPerceived, 1 )

    MOCK_METHOD( IsAFriend, 1 )
    MOCK_METHOD( IsAnEnemy, 1 )
    MOCK_METHOD( IsNeutral, 1 )

    MOCK_METHOD( FindKnowledgeGroup, 1 )
    MOCK_METHOD( FindCrowdKnowledgeGroup, 0 )
    MOCK_METHOD( RegisterKnowledgeGroup, 1 )
    MOCK_METHOD( UnregisterKnowledgeGroup, 1 )

    MOCK_METHOD( InitializeDiplomacy, 1 )
    MOCK_METHOD( ChangeDiplomacy, 2 )
    MOCK_METHOD( ApplyOnKnowledgeGroup, 1 )

    MOCK_METHOD( GetID, 0 )
    MOCK_METHOD( GetName, 0 )
    MOCK_METHOD( GetKnowledge, 0 )
    MOCK_METHOD( GetColor, 0 )
    MOCK_METHOD( GetKnowledgeGroups, 0 )
    MOCK_METHOD( GetObjects, 0 )

    MOCK_METHOD( OnReceiveUnitMagicAction, 1 )

    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & boost::serialization::base_object< MIL_Army_ABC >( *this );
    }
};

BOOST_CLASS_EXPORT_KEY( MockArmy )

#endif // __MockArmy_h_
