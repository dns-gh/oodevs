// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockMIL_EntityManager_ABC_h_
#define __MockMIL_EntityManager_ABC_h_

#include "Entities/MIL_EntityManager_ABC.h"

// =============================================================================
/** @class  MockMIL_EntityManager_ABC
    @brief  MockMIL_EntityManager_ABC
*/
// Created: LDC 2009-06-09
// =============================================================================
MOCK_BASE_CLASS( MockMIL_EntityManager_ABC, MIL_EntityManager_ABC )
{
    MOCK_METHOD( FindAgentPion, 1 );
    MOCK_METHOD( FindAutomate, 1 );
    MOCK_METHOD( FindObject, 1 );
    MOCK_METHOD( FindPopulation, 1 );
    MOCK_METHOD( FindKnowledgeGroup, 1 ); // LTO
    MOCK_METHOD( FindObjectType, 1 );

    MOCK_METHOD_EXT( CreateObject, 3, MIL_Object_ABC*( const std::string&, MIL_Army_ABC*, const TER_Localisation& ), CreateObjectFromType );
    MOCK_METHOD_EXT( CreateObject, 2, MIL_Object_ABC*( MIL_Army_ABC*, const MIL_ObjectBuilder_ABC& ), CreateObjectFromBuilder );
};

#endif // __MockMIL_EntityManager_ABC_h_
