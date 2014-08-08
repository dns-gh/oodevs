// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockEntityResolver_h_
#define __MockEntityResolver_h_

#include "clients_kernel/EntityResolver_ABC.h"

// =============================================================================
/** @class  MockEntityResolver
    @brief  MockEntityResolver
*/
// Created: SBO 2010-05-10
// =============================================================================
MOCK_BASE_CLASS( MockEntityResolver, kernel::EntityResolver_ABC )
{
    MOCK_METHOD( FindAgent, 1 );
    MOCK_METHOD( GetAgent, 1 );
    MOCK_METHOD( FindAutomat, 1 );
    MOCK_METHOD( GetAutomat, 1 );
    MOCK_METHOD( FindKnowledgeGroup, 1 );
    MOCK_METHOD( GetKnowledgeGroup, 1 );
    MOCK_METHOD( FindObject, 1 );
    MOCK_METHOD( GetObject, 1 );
    MOCK_METHOD( FindUrbanObject, 1 );
    MOCK_METHOD( GetUrbanObject, 1 );
    MOCK_METHOD( FindInhabitant, 1 );
    MOCK_METHOD( GetInhabitant, 1 );
    MOCK_METHOD( FindPopulation, 1 );
    MOCK_METHOD( GetPopulation, 1 );
    MOCK_METHOD( FindFormation, 1 );
    MOCK_METHOD( GetFormation, 1 );
    MOCK_METHOD( FindTeam, 1 );
    MOCK_METHOD( GetTeam, 1 );
    MOCK_METHOD( FindEntity, 1 );
};

#endif // __MockEntityResolver_h_
