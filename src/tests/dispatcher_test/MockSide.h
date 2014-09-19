// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockSide_h_
#define __MockSide_h_

#include "dispatcher/Team_ABC.h"

// =============================================================================
/** @class  MockSide
    @brief  MockSide
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockSide, dispatcher::Team_ABC )
{
    explicit MockSide( unsigned int id ) : dispatcher::Team_ABC( id ) {}

    // Entity_ABC
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 3 );
    MOCK_METHOD( Activate, 1 );

    // Team_ABC
    MOCK_METHOD( GetKarma, 0 );
    MOCK_METHOD( Register, 1, void ( dispatcher::Formation_ABC& ), RegisterFormation );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Formation_ABC& ), RemoveFormation );
    MOCK_METHOD( Register, 1, void ( dispatcher::Population_ABC& ), RegisterPopulation );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Population_ABC& ), RemovePopulation );
    MOCK_METHOD( Register, 1, void ( dispatcher::Object_ABC& ), RegisterObject );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Object_ABC& ), RemoveObject );
    MOCK_METHOD( Register, 1, void ( dispatcher::KnowledgeGroup_ABC& ), RegisterKnowledgeGroup );
    MOCK_METHOD( Remove, 1, void ( dispatcher::KnowledgeGroup_ABC& ), RemoveKnowledgeGroup );
    MOCK_METHOD( Register, 1, void ( dispatcher::Inhabitant_ABC& ), RegisterInhabitant );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Inhabitant_ABC& ), RemoveInhabitant );
    MOCK_METHOD( GetExtension, 2 );

    // Sendable
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );
};

#endif // __MockSide_h_
