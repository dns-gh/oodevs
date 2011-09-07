// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_test_MockTeam_h
#define plugins_hla_test_MockTeam_h

namespace dispatcher
{
// =============================================================================
/** @class  MockTeam
    @brief  Mock team
*/
// Created: SLI 2011-09-07
// =============================================================================
MOCK_BASE_CLASS( MockTeam, dispatcher::Team_ABC )
{
public:
    MockTeam( unsigned long id )
        : Team_ABC( id )
    {}
    MOCK_METHOD( SendCreation, 1 );
    MOCK_METHOD( SendFullUpdate, 1 );
    MOCK_METHOD( SendDestruction, 1 );
    MOCK_METHOD( Accept, 1 );
    MOCK_METHOD( GetKarma, 0 );
    MOCK_METHOD_EXT( Register, 1, void( dispatcher::Formation_ABC& ), RegisterFormation );
    MOCK_METHOD_EXT( Remove  , 1, void( dispatcher::Formation_ABC& ), RemoveFormation );
    MOCK_METHOD_EXT( Register, 1, void( dispatcher::Population_ABC& ), RegisterPopulation );
    MOCK_METHOD_EXT( Remove  , 1, void( dispatcher::Population_ABC& ), RemovePopulation );
    MOCK_METHOD_EXT( Register, 1, void( dispatcher::Inhabitant_ABC& ), RegisterInhabitant );
    MOCK_METHOD_EXT( Remove  , 1, void( dispatcher::Inhabitant_ABC& ), RemoveInhabitant );
    MOCK_METHOD_EXT( Register, 1, void( dispatcher::Object_ABC& ), RegisterObject );
    MOCK_METHOD_EXT( Remove  , 1, void( dispatcher::Object_ABC& ), RemoveObject );
    MOCK_METHOD_EXT( Register, 1, void( dispatcher::KnowledgeGroup_ABC& ), RegisterKnowledgeGroup );
    MOCK_METHOD_EXT( Remove  , 1, void( dispatcher::KnowledgeGroup_ABC& ), RemoveKnowledgeGroup );
};

}

#endif // plugins_hla_test_MockTeam_h
