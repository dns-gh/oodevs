// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockAutomat_h_
#define __MockAutomat_h_

#include "dispatcher/Automat_ABC.h"

// =============================================================================
/** @class  MockAutomat
    @brief  MockAutomat
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockAutomat, dispatcher::Automat_ABC )
{
    explicit MockAutomat( unsigned int id ) : dispatcher::Automat_ABC( id ) {}

    // kernel::Entity_ABC
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 3 );
    MOCK_METHOD( Activate, 1 );

    // kernel::Automat_ABC
    MOCK_METHOD( GetLogisticEntity, 0 );
    MOCK_METHOD( GetType, 0 );

    // dispatcher::Automat_ABC
    MOCK_METHOD( GetTeam, 0 );
    MOCK_METHOD( GetKnowledgeGroup, 0 );
    MOCK_METHOD( Register, 1, void ( dispatcher::Automat_ABC& ), RegisterAutomat );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Automat_ABC& ), RemoveAutomat );
    MOCK_METHOD( Register, 1, void ( dispatcher::Agent_ABC& ), RegisterAgent );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Agent_ABC& ), RemoveAgent );
    MOCK_METHOD( GetAgents, 0 );
    MOCK_METHOD( GetParentAutomat, 0 );
    MOCK_METHOD( GetFormation, 0 );
    MOCK_METHOD( IsEngaged, 0 );
    MOCK_METHOD( SetSuperior, 1, void ( dispatcher::Formation_ABC& ), SetSuperiorFormation );
    MOCK_METHOD( SetSuperior, 1, void ( dispatcher::Automat_ABC& ), SetSuperiorAutomat );
    MOCK_METHOD( GetApp6Symbol, 0);

    // dispatcher::Sendable< kernel::Automat_ABC >
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );

    // dispatcher::LogisticHierarchyOwner_ABC
    MOCK_METHOD( Send, 1, void ( sword::ParentEntity& ), LogisticHierarchyOwnerSend );
};

#endif // __MockAutomat_h_
