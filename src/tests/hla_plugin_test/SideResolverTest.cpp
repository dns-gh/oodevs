// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/SideResolver.h"
#include "clients_kernel/Karma.h"
#include "MockLogger.h"
#include "MockModel.h"
#include "MockResolver.h"
#include "MockTeam.h"
#include <boost/mpl/list.hpp>

using namespace plugins::hla;

namespace
{
    struct Fixture
    {
        typedef std::vector< dispatcher::MockTeam* > TeamContainer;

        dispatcher::MockModel model_;
        dispatcher::MockLogger logger_;
    };
    
}

BOOST_FIXTURE_TEST_CASE( side_resolver_read_teams, Fixture )
{
    tools::MockResolver< dispatcher::Team_ABC > teamResolver;
    TeamContainer emptyTeams;
    tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >* teamsIt = new tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >( emptyTeams );
    tools::Iterator< const dispatcher::Team_ABC& > it( teamsIt );

    MOCK_EXPECT( model_.Sides ).once().returns( boost::ref( teamResolver ) );
    MOCK_EXPECT( teamResolver.CreateIterator ).once().returns( it );
    SideResolver sideResolver( model_, logger_ );

    dispatcher::MockTeam t1( 42 );
    BOOST_CHECK_THROW( sideResolver.ResolveForce( t1.GetId() ),  std::runtime_error );
    BOOST_CHECK_THROW( sideResolver.ResolveTeam( rpr::Friendly ),  std::runtime_error );
}

namespace 
{
    template< rpr::ForceIdentifier F >
    struct ForceIdentifierHandler
    {
        static rpr::ForceIdentifier forceId;
        static kernel::Karma GetKarma()
        {
            switch( forceId )
            {
            case rpr::Friendly:
                return kernel::Karma::friend_;
                break;
            case rpr::Opposing:
                return kernel::Karma::enemy_;
                break;
            case rpr::Neutral:
                return kernel::Karma::neutral_;
                break;
            };
            return kernel::Karma::unknown_;
        }
    };
    template< rpr::ForceIdentifier F >  rpr::ForceIdentifier ForceIdentifierHandler< F >::forceId = F;
    typedef boost::mpl::list< ForceIdentifierHandler< rpr::Friendly >, 
                              ForceIdentifierHandler< rpr::Opposing >, 
                              ForceIdentifierHandler< rpr::Neutral > > test_types;
}

namespace
{
    bool ReturnExtension( const std::string& name, std::string& value, const std::string& expected, const std::string& returnValue, bool b )
    {
        BOOST_CHECK_EQUAL( name, expected );
        if( b )
            value = returnValue;
        return b;
    }
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE( side_resolver_creates_default_mapping, T, test_types, Fixture )
{
    tools::MockResolver< dispatcher::Team_ABC > teamResolver;
    TeamContainer emptyTeams;

    dispatcher::MockTeam* fr1( new dispatcher::MockTeam( 42 ) );
    MOCK_EXPECT( fr1->GetExtension ).once().calls( boost::bind( &ReturnExtension, _1, _2, "HlaForceIdentifier", "4", false ) );
    MOCK_EXPECT( fr1->GetKarma ).once().returns( T::GetKarma() );
    emptyTeams.push_back( fr1 );
    tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >* teamsIt = new tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >( emptyTeams );
    tools::Iterator< const dispatcher::Team_ABC& > it( teamsIt );

    MOCK_EXPECT( model_.Sides ).once().returns( boost::ref( teamResolver ) );
    MOCK_EXPECT( teamResolver.CreateIterator ).once().returns( it );
    SideResolver sideResolver( model_, logger_ );
    
    dispatcher::MockTeam t2( 43 );
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam( T::forceId ), fr1->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveForce( fr1->GetId() ), T::forceId );

    MOCK_EXPECT( logger_.LogError ).exactly( 2 );
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam( rpr::Opposing ), fr1->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam( rpr::Neutral ), fr1->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam( rpr::Friendly ), fr1->GetId() );

    BOOST_CHECK_THROW( sideResolver.ResolveForce( t2.GetId() ), std::runtime_error );
}

BOOST_FIXTURE_TEST_CASE( side_resolver_creates_default_with_collision, Fixture )
{
    tools::MockResolver< dispatcher::Team_ABC > teamResolver;
    TeamContainer emptyTeams;

    dispatcher::MockTeam* fr1( new dispatcher::MockTeam( 42 ) );
    MOCK_EXPECT( fr1->GetExtension ).once().calls( boost::bind( &ReturnExtension, _1, _2, "HlaForceIdentifier", "4", false ) );
    MOCK_EXPECT( fr1->GetKarma ).once().returns( kernel::Karma::friend_ );
    emptyTeams.push_back( fr1 );

    dispatcher::MockTeam* fr2( new dispatcher::MockTeam( 43 ) );
    MOCK_EXPECT( fr2->GetExtension ).once().calls( boost::bind( &ReturnExtension, _1, _2, "HlaForceIdentifier", "4", false ) );
    MOCK_EXPECT( fr2->GetKarma ).once().returns( kernel::Karma::friend_ );
    emptyTeams.push_back( fr2 );

    tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >* teamsIt = new tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >( emptyTeams );
    tools::Iterator< const dispatcher::Team_ABC& > it( teamsIt );
    
    MOCK_EXPECT( model_.Sides ).once().returns( boost::ref( teamResolver ) );
    MOCK_EXPECT( teamResolver.CreateIterator ).once().returns( it );
    SideResolver sideResolver( model_, logger_ );
    
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam(  rpr::Friendly ), fr1->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveForce( fr1->GetId() ), rpr::Friendly );
    
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam(  rpr::Friendly_2 ), fr2->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveForce( fr2->GetId() ), rpr::Friendly_2 );
}

BOOST_FIXTURE_TEST_CASE( side_resolver_reads_mapping_and_uses_default, Fixture )
{
    tools::MockResolver< dispatcher::Team_ABC > teamResolver;
    TeamContainer emptyTeams;

    dispatcher::MockTeam* team1( new dispatcher::MockTeam( 42 ) );
    dispatcher::MockTeam* team2( new dispatcher::MockTeam( 43 ) );
    dispatcher::MockTeam* team3( new dispatcher::MockTeam( 44 ) );
    
    MOCK_EXPECT( team1->GetExtension ).once().calls( boost::bind( &ReturnExtension, _1, _2, "HlaForceIdentifier", "4", true ) ); // rpr::Friendly_2
    MOCK_EXPECT( team2->GetExtension ).once().calls( boost::bind( &ReturnExtension, _1, _2, "HlaForceIdentifier", "5", true ) ); // rpr::Opposing_2
    MOCK_EXPECT( team3->GetExtension ).once().calls( boost::bind( &ReturnExtension, _1, _2, "HlaForceIdentifier", "4", false ) );
    MOCK_EXPECT( team3->GetKarma ).once().returns( kernel::Karma::friend_ );
    emptyTeams.push_back( team1 );
    emptyTeams.push_back( team2 );
    emptyTeams.push_back( team3 );

    tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >* teamsIt = new tools::SimpleIterator< const dispatcher::Team_ABC&, TeamContainer >( emptyTeams );
    tools::Iterator< const dispatcher::Team_ABC& > it( teamsIt );

    MOCK_EXPECT( model_.Sides ).once().returns( boost::ref( teamResolver ) );
    MOCK_EXPECT( teamResolver.CreateIterator ).once().returns( it );
    SideResolver sideResolver( model_, logger_ );
    
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam( rpr::Friendly_2 ), team1->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveForce( team1->GetId() ), rpr::Friendly_2 );
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam( rpr::Opposing_2 ), team2->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveForce( team2->GetId() ), rpr::Opposing_2 );
    BOOST_CHECK_EQUAL( sideResolver.ResolveTeam( rpr::Friendly ), team3->GetId() );
    BOOST_CHECK_EQUAL( sideResolver.ResolveForce( team3->GetId() ), rpr::Friendly );
}