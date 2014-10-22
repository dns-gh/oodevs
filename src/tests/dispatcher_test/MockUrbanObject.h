// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockUrbanObject_h_
#define __MockUrbanObject_h_

#include "dispatcher/UrbanObject_ABC.h"

// =============================================================================
/** @class  MockUrbanObject
    @brief  MockUrbanObject
*/
// Created: PHC 2010-07-22
// =============================================================================
MOCK_BASE_CLASS( MockUrbanObject, dispatcher::UrbanObject_ABC )
{
    explicit MockUrbanObject( unsigned int id ) : dispatcher::UrbanObject_ABC( id ) {}

    // kernel::Entity_ABC
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 3 );
    MOCK_METHOD( Activate, 1 );

    // dispatcher::Sendable< kernel::UrbanObject_ABC >
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );

    MOCK_METHOD( ApplyTemplate, 1 );
    MOCK_METHOD( UpdateTemplate, 1 );
    MOCK_METHOD( IsUpdatingTemplate, 0 );

    MOCK_METHOD( GetLivingSpace, 1);
    MOCK_METHOD( GetHumansMap, 0 );
};

#endif // __MockUrbanObject_h_
