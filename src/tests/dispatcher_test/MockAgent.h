// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockAgent_h_
#define __MockAgent_h_

#include "dispatcher/Agent_ABC.h"

// =============================================================================
/** @class  MockAgent
    @brief  MockAgent
*/
// Created: SBO 2010-06-07
// =============================================================================
MOCK_BASE_CLASS( MockAgent, dispatcher::Agent_ABC )
{
    explicit MockAgent( unsigned int id ) : dispatcher::Agent_ABC( id ) {}

    // kernel::Entity_ABC
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 2 );
    MOCK_METHOD( Activate, 1 );

    // dispatcher::Sendable< kernel::Automat_ABC >
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );
};

#endif // __MockAgent_h_
