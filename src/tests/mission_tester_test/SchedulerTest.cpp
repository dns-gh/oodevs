// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "mission_tester_test_pch.h"
#include "mission_tester/Criteria.h"
#include "mission_tester/Filter_ABC.h"
#include "mission_tester/Filter.h"
#include "mission_tester/Schedulable_ABC.h"
#include "mission_tester/Scheduler.h"

using namespace mission_tester;

MOCK_BASE_CLASS( MockSchedulable, Schedulable_ABC )
{
    MOCK_METHOD( Trigger, 1 )
    MOCK_METHOD( Matches, 1 )
};

BOOST_AUTO_TEST_CASE( scheduler_schedules_a_schedulable )
{
    Scheduler scheduler( "any" );
    MockSchedulable schedulable;
    MOCK_EXPECT( filter, Accepts ).once().with( mock::same( schedulable ) ).returns( true );
    scheduler.Schedule( schedulable );
}

namespace
{
    struct Schedulable : public Schedulable_ABC
    {
        Schedulable( const std::string& type )
            : type_( type )
        {}
        virtual bool Trigger( State_ABC& /*state*/ ){ return true; }
        virtual bool Matches( const Criteria& criteria ) const
        { 
            if( !criteria.Matches( type_ ) )
                throw std::runtime_error( "bad criterion" );
            return true;
        }
        const std::string type_;
    };
}

struct FilterFixture
{
    FilterFixture()

        : criteria_            ( "agent" )
        , agentFilter_         ( criteria_ )
        , agentScheduler_      ( agentFilter_ )
        , agentSchedulable_    ( "agent" )
        , automatonSchedulable_( "automaton" )
    {}
    Criteria criteria_;
    Filter agentFilter_;
    Scheduler agentScheduler_;
    Schedulable agentSchedulable_;
    Schedulable automatonSchedulable_;
};

BOOST_FIXTURE_TEST_CASE( agent_filter_allows_only_agents, FilterFixture )
{
    agentScheduler_.Schedule( agentSchedulable_ );
    BOOST_CHECK_THROW( agentScheduler_.Schedule( automatonSchedulable_ ), std::runtime_error );
}