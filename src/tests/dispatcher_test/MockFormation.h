// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockFormation_h_
#define __MockFormation_h_

#include "dispatcher/Formation_ABC.h"

// =============================================================================
/** @class  MockFormation
    @brief  MockFormation
*/
// Created: SBO 2010-06-02
// =============================================================================
MOCK_BASE_CLASS( MockFormation, dispatcher::Formation_ABC )
{
    explicit MockFormation( unsigned int id ) : dispatcher::Formation_ABC( id ) {}

    // Entity_ABC
    MOCK_METHOD( GetName, 0 );
    MOCK_METHOD( GetId, 0 );
    MOCK_METHOD( GetTypeName, 0 );
    MOCK_METHOD( Select, 1 );
    MOCK_METHOD( ContextMenu, 3 );
    MOCK_METHOD( Activate, 1 );
    MOCK_METHOD( Accept, 1 );

    // Formation_ABC
    MOCK_METHOD( GetLevel, 0 );
    MOCK_METHOD( GetParent, 0 );
    MOCK_METHOD( GetTeam, 0 );
    MOCK_METHOD( GetFormations, 0 );
    MOCK_METHOD( GetAutomates, 0 );
    MOCK_METHOD( Register, 1, void ( dispatcher::Formation_ABC& ), RegisterFormation );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Formation_ABC& ), RemoveFormation );
    MOCK_METHOD( Register, 1, void ( dispatcher::Automat_ABC& ), RegisterAutomat );
    MOCK_METHOD( Remove, 1, void ( dispatcher::Automat_ABC& ), RemoveAutomat );
    MOCK_METHOD( SetSuperior, 1, void ( dispatcher::Formation_ABC& ), SetSuperiorFormation );
    MOCK_METHOD( SetSuperior, 1, void ( dispatcher::Team_ABC& ), SetSuperiorTeam );
    MOCK_METHOD( GetLogisticEntity, 0 );
    MOCK_METHOD( GetApp6Symbol, 0);

    // Sendable
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );

    MOCK_METHOD( Send, 1 );
};

#endif // __MockFormation_h_
