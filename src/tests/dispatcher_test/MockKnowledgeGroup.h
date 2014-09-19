// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockKnowledgeGroup_h_
#define __MockKnowledgeGroup_h_

#include "dispatcher/KnowledgeGroup_ABC.h"

// =============================================================================
/** @class  MockKnowledgeGroup
    @brief  MockKnowledgeGroup
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockKnowledgeGroup, dispatcher::KnowledgeGroup_ABC )
{
    explicit MockKnowledgeGroup( unsigned int id ) : dispatcher::KnowledgeGroup_ABC( id ) {}

    // Entity_ABC
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 3 );
    MOCK_METHOD( Activate, 1 );

    // kernel::KnowledgeGroup_ABC
    // LTO begin
    MOCK_METHOD( IsActivated, 0 );
    // LTO end
    MOCK_METHOD( IsCrowd, 0 );

    // dispatcher::KnowledgeGroup_ABC
    MOCK_METHOD( GetTeam, 0 );
    // LTO begin
    MOCK_METHOD( Register, 1, void ( dispatcher::KnowledgeGroup_ABC& ), RegisterKnowledgeGroup );
    MOCK_METHOD( Remove, 1, void ( dispatcher::KnowledgeGroup_ABC& ), RemoveKnowledgeGroup );
    // LTO end
    MOCK_METHOD( Register, 1, void ( dispatcher::Automat_ABC& ), RegisterAutomat );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Automat_ABC& ), RemoveAutomat );
    MOCK_METHOD( ChangeSuperior, 1 );

    // Sendable
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );
};

#endif // __MockKnowledgeGroup_h_
