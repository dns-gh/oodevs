// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/FormationCreater.h"
#include "MockResolver.h"
#include "MockContextHandler.h"
#include "MockTeam.h"
#include "protocol/Simulation.h"
#include "protocol/SimulationSenders.h"
#include <boost/shared_ptr.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <vector>

using namespace plugins::hla;

namespace
{
    typedef std::vector< unsigned long > T_Identifiers;
    typedef boost::shared_ptr< dispatcher::MockTeam > T_Team;
    typedef std::vector< T_Team > T_Teams;

    class Fixture
    {
    public:
        Fixture()
            : party( 42u )
        {}
        template< typename T_Result, typename T_Mock, typename T_Vector >
        tools::Iterator< const T_Result& > MakeIterator( const T_Identifiers& identifiers, T_Vector& elements )
        {
            BOOST_FOREACH( unsigned long identifier, identifiers )
                elements.push_back( boost::make_shared< T_Mock >( identifier ) );
            tools::SimpleIterator< const T_Result&, T_Vector >* it = new tools::SimpleIterator< const T_Result&, T_Vector >( elements );
            return tools::Iterator< const T_Result& >( it );
        }
        tools::Iterator< const dispatcher::Team_ABC& > MakeTeamIterator( const T_Identifiers& identifiers )
        {
            return MakeIterator< dispatcher::Team_ABC, dispatcher::MockTeam, T_Teams >( identifiers, teams );
        }
        const unsigned long party;
        tools::MockResolver< dispatcher::Team_ABC > teamResolver;
        MockContextHandler< sword::FormationCreation > formationCreation;
        T_Teams teams;
    };
}

BOOST_FIXTURE_TEST_CASE( formation_creater_creates_formation_for_each_party, Fixture )
{
    MOCK_EXPECT( teamResolver.CreateIterator ).once().returns( MakeTeamIterator( boost::assign::list_of( party ) ) );
    simulation::UnitMagicAction actual;
    MOCK_EXPECT( formationCreation.Send ).once().with( mock::retrieve( actual ), mock::any );
    FormationCreater formationCreater( teamResolver, formationCreation );
    mock::verify();
    const sword::UnitMagicAction& action = actual();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::formation_creation );
    BOOST_CHECK_EQUAL( action.tasker().party().id(), party );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 4 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).areal(), 6 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 1 ).value( 0 ).acharstr(), "HLA distant formation" );
    BOOST_CHECK( action.parameters().elem( 2 ).null_value() );
}
