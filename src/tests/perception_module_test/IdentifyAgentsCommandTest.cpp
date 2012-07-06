// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "perception_module_test_pch.h"
#include "ModuleFixture.h"
#include <boost/bind/apply.hpp>

class TER_Localisation;
class MIL_Agent_ABC;

BOOST_FIXTURE_TEST_CASE( identify_all_agents_in_zone_command_notifies_perceptions, sword::perception::ModuleFixture )
{
    const size_t identifier = 3;
    const SWORD_Model* other = core::Convert( &model[ "entities/other" ] );
    model[ "entities/other/pion" ].SetUserData< MIL_Agent_ABC* >( reinterpret_cast< MIL_Agent_ABC* >( 43 ) );
    const TER_Localisation* localization = reinterpret_cast< TER_Localisation* >( 42 );
    core::Model& zone = model[ "entities" ][ identifier ][ "perceptions/notifications/agents-in-zone" ];
    MOCK_EXPECT( GetAgentListWithinLocalisation ).once().with( mock::any, localization, mock::any, mock::any ).calls( boost::bind( boost::apply< void >(), _3, other, _4 ) );
    ExpectEffect( zone, sword::test::MakeModel()
                          [ sword::test::MakeModel( "level", 3 )
                                                  ( "target", 43 )
                                                  ( "recorded", false ) ] );
    commands.Start( "identify all agents in zone",
        core::MakeModel( "identifier", identifier )
                       ( "localization", core::MakeUserData< const TER_Localisation* >( localization ) ) );
    commands.Execute();
}
