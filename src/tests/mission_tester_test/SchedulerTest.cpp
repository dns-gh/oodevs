// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "mission_tester_test_pch.h"
#include "mission_tester/Filter_ABC.h"
#include "mission_tester/FilterFactory_ABC.h"
#include "mission_tester/Schedulable_ABC.h"
#include "mission_tester/Scheduler.h"

using namespace mission_tester;

MOCK_BASE_CLASS( MockSchedulable, Schedulable_ABC )
{
    MOCK_METHOD( Trigger, 1 )
    MOCK_METHOD( Matches, 1 )
    MOCK_METHOD( StartMission, 1 )
};

MOCK_BASE_CLASS( MockFactory, FilterFactory_ABC )
{
    MOCK_METHOD( Create, 1 )
};

MOCK_BASE_CLASS( MockFilter, Filter_ABC )
{
    MOCK_METHOD( Accepts, 1 )
};

namespace
{
    struct Schedulable : public Schedulable_ABC
    {
        Schedulable( const std::string& type )
            : type_( type )
        {}
        virtual bool Trigger( State_ABC& /*state*/ ){ return true; }
        virtual bool Matches( const Filter_ABC& filter ) const
        {
            if( !filter.Accepts( type_ ) )
                throw std::runtime_error( "bad criterion" );
            return true;
        }
        virtual std::string SchedulableName() const { return "testAgent"; }
        virtual bool StartMission( Exercise& /*exercise*/ ) { return true; }
        const std::string type_;
    };
}

BOOST_AUTO_TEST_CASE( scheduler_schedules_a_schedulable )
{
    Filter_ABC* filter = new MockFilter();
    Scheduler scheduler( std::auto_ptr< Filter_ABC >( filter ), 500u );
    boost::shared_ptr< MockSchedulable > schedulable( new MockSchedulable );
    MOCK_EXPECT( schedulable, Matches ).once().with( mock::same( *filter ) ).returns( true );
    scheduler.Schedule( schedulable );
}

namespace
{
    FilterFactory_ABC* FilterFactoryStub()
    {
        MockFilter* filter = new MockFilter;
        MockFactory* factory = new MockFactory;
        MOCK_EXPECT( factory, Create ).once().with( "agent" ).returns( filter );
        MOCK_EXPECT( filter, Accepts ).once().with( "agent" ).returns( true );
        return factory;
    }

    struct FilterFixture
    {
        FilterFixture()
            : factory_             ( FilterFactoryStub() )
            , agentScheduler_      ( factory_->Create( "agent" ), 500u )
            , agentSchedulable_    ( new Schedulable( "agent" ) )
            , automatonSchedulable_( new Schedulable( "automat" ) )
        {}
        MockFilter* filter_;
        std::auto_ptr< FilterFactory_ABC > factory_;
        Scheduler agentScheduler_;
        boost::shared_ptr< Schedulable_ABC > agentSchedulable_;
        boost::shared_ptr< Schedulable_ABC > automatonSchedulable_;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_filter_allows_only_agents, FilterFixture )
{
    agentScheduler_.Schedule( agentSchedulable_ );
}
