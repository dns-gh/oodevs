// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockObjectKnowledge_h_
#define __MockObjectKnowledge_h_

#include "dispatcher/ObjectKnowledge_ABC.h"

// =============================================================================
/** @class  MockObjectKnowledge
    @brief  MockObjectKnowledge
*/
// Created: PHC 2010-07-20
// =============================================================================
MOCK_BASE_CLASS( MockObjectKnowledge, dispatcher::ObjectKnowledge_ABC )
{
    explicit MockObjectKnowledge( unsigned int id ) : dispatcher::ObjectKnowledge_ABC( id ) {}

    // kernel::Entity_ABC
    MOCK_METHOD( GetEntity, 0 );
    MOCK_METHOD( GetOwner, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 2 );
    MOCK_METHOD( Activate, 1 );
    MOCK_METHOD( GetEntityId, 0 );

    // dispatcher::Sendable< kernel::ObjectKnowledge_ABC >
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );
};

#endif // __MockObjectKnowledge_h_
