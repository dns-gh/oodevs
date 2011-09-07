// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_test_MockKnowledgeGroup_h
#define plugins_hla_test_MockKnowledgeGroup_h

namespace dispatcher
{
// =============================================================================
/** @class  MockKnowledgeGroup
    @brief  Mock knowledge group
*/
// Created: SLI 2011-09-07
// =============================================================================
MOCK_BASE_CLASS( MockKnowledgeGroup, KnowledgeGroup_ABC )
{
    MockKnowledgeGroup( unsigned long id )
        : KnowledgeGroup_ABC( id )
    {}
    MOCK_METHOD( GetTeam, 0 );
    MOCK_METHOD_EXT( Register, 1, void( dispatcher::KnowledgeGroup_ABC& ), RegisterKnowledgeGroup );
    MOCK_METHOD_EXT( Remove  , 1, void( dispatcher::KnowledgeGroup_ABC& ), RemoveKnowledgeGroup );
    MOCK_METHOD_EXT( Register, 1, void( dispatcher::Automat_ABC& ), RegisterAutomat );
    MOCK_METHOD_EXT( Remove  , 1, void( dispatcher::Automat_ABC& ), RemoveAutomat );
    MOCK_METHOD( ChangeSuperior, 1 );
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );
    MOCK_METHOD( IsActivated, 0 );
};

}

#endif // plugins_hla_test_MockKnowledgeGroup_h
