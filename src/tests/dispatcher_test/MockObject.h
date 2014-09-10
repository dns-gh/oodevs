// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockObject_h_
#define __MockObject_h_

#include "dispatcher/Object_ABC.h"

// =============================================================================
/** @class  MockObject
    @brief  MockObject
*/
// Created: SBO 2010-06-09
// =============================================================================
MOCK_BASE_CLASS( MockObject, dispatcher::Object_ABC )
{
    explicit MockObject( unsigned int id ) : dispatcher::Object_ABC( id ) {}

    // kernel::Entity_ABC
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 3 );
    MOCK_METHOD( Activate, 1 );

    // kernel::Object_ABC
    MOCK_METHOD( Display, 1 );
    MOCK_METHOD( GetType, 0 );
    MOCK_METHOD( GetExtension, 2 );
    MOCK_METHOD( GetSymbol, 0 );

    // dispatcher::Sendable< kernel::Object_ABC >
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );

    // dispatcher::Object_ABC
    MOCK_METHOD( GetTeam, 0 );
    MOCK_METHOD( GetLocalisation, 0 );
    MOCK_METHOD( GetAttributes, 0 );
};

#endif // __MockObject_h_
