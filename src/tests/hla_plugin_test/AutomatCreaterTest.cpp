// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/AutomatCreater.h"
#include "MockContextHandler.h"
#include "MockResolver.h"
#include "MockTeam.h"
#include "MockKnowledgeGroup.h"
#include "MockUnitTypeResolver.h"
#include "protocol/Simulation.h"
#include "protocol/SimulationSenders.h"
#include <boost/shared_ptr.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    typedef std::vector< unsigned long > T_Identifiers;
    typedef boost::shared_ptr< dispatcher::MockKnowledgeGroup > T_KnowledgeGroup;
    typedef std::vector< T_KnowledgeGroup > T_KnowledgeGroups;

    class Fixture
    {
    public:
        Fixture()
            : xis                     ( "<configuration>"
                                        "    <types>"
                                        "        <automat>automat type name</automat>"
                                        "    </types>"
                                        "</configuration>")
            , formationCreationHandler( 0 )
            , party                   ( 42 )
            , formation               ( 43 )
            , team                    ( party )
        {
            xis >> xml::start( "configuration" );
            MOCK_EXPECT( formationCreation.Register ).once().with( mock::retrieve( formationCreationHandler ) );
            MOCK_EXPECT( formationCreation.Unregister ).once();
        }
        template< typename T_Result, typename T_Mock, typename T_Vector >
        tools::Iterator< const T_Result& > MakeIterator( const T_Identifiers& identifiers, T_Vector& elements )
        {
            BOOST_FOREACH( unsigned long identifier, identifiers )
                elements.push_back( boost::make_shared< T_Mock >( identifier ) );
            tools::SimpleIterator< const T_Result&, T_Vector >* it = new tools::SimpleIterator< const T_Result&, T_Vector >( elements );
            return tools::Iterator< const T_Result& >( it );
        }
        tools::Iterator< const dispatcher::KnowledgeGroup_ABC& > MakeKnowledgeGroupIterator( const T_Identifiers& identifiers )
        {
            return MakeIterator< dispatcher::KnowledgeGroup_ABC, dispatcher::MockKnowledgeGroup, T_KnowledgeGroups >( identifiers, groups );
        }
        void ConfigureKnowledgeGroups()
        {
            MOCK_EXPECT( knowledgeGroupResolver.CreateIterator ).once().returns( MakeKnowledgeGroupIterator( boost::assign::list_of( party ) ) );
            BOOST_FOREACH( T_KnowledgeGroup group, groups )
                MOCK_EXPECT( group->GetTeam ).once().returns( boost::ref( team ) );
        }
        sword::FormationCreation MakeFormationCreationMessage()
        {
            sword::FormationCreation message;
            message.mutable_party()->set_id( party );
            message.mutable_formation()->set_id( formation );
            return message;
        }
        xml::xistringstream xis;
        MockContextHandler< sword::FormationCreation > formationCreation;
        MockContextHandler< sword::AutomatCreation > automatCreation;
        MockUnitTypeResolver automatResolver;
        tools::MockResolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroupResolver;
        ResponseObserver_ABC< sword::FormationCreation >* formationCreationHandler;
        T_KnowledgeGroups groups;
        unsigned long party;
        unsigned long formation;
        dispatcher::MockTeam team;
    };
}

BOOST_FIXTURE_TEST_CASE( automat_creater_checks_remote_automat_type_id_existence, Fixture )
{
    mock::reset( formationCreation );
    MOCK_EXPECT( automatResolver.ResolveName ).once().returns( 0 );
    BOOST_CHECK_THROW( AutomatCreater automatCreater( xis, formationCreation, automatCreation, automatResolver, knowledgeGroupResolver ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( automat_creater_sends_automat_creation_message_when_receiving_formation_creation, Fixture )
{
    MOCK_EXPECT( automatResolver.ResolveName ).once().with( "automat type name" ).returns( 42u );
    ConfigureKnowledgeGroups();
    AutomatCreater automatCreater( xis, formationCreation, automatCreation, automatResolver, knowledgeGroupResolver );
    BOOST_REQUIRE( formationCreationHandler );
    simulation::UnitMagicAction actual;
    MOCK_EXPECT( automatCreation.Send ).once().with( mock::retrieve( actual ), mock::any );
    formationCreationHandler->Notify( MakeFormationCreationMessage(), "default_remote_formation" );
    const sword::UnitMagicAction& action = actual();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::automat_creation );
    BOOST_CHECK_EQUAL( action.tasker().formation().id(), formation );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 4 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).identifier(), 42u );
    BOOST_CHECK_EQUAL( action.parameters().elem( 1 ).value( 0 ).identifier(), party );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), "HLA distant automat" );
}
