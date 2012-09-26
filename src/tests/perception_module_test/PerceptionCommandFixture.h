// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PERCEPTION_COMMAND_FIXTURE_H
#define PERCEPTION_COMMAND_FIXTURE_H

#include "PerceptionViewFixture.h"

namespace sword
{
namespace perception
{
    struct PerceptionCommandFixture : public sword::perception::PerceptionViewFixture
    {
        PerceptionCommandFixture()
            : target( 123 )
        {
            MOCK_EXPECT( GetUrbanObjectListWithinCircle );
            MOCK_EXPECT( AppendAddedKnowledge );
            MOCK_EXPECT( GetAgentListWithinCircle );
            MOCK_EXPECT( GetObjectListWithinCircle );
            MOCK_EXPECT( GetConcentrationListWithinCircle );
            MOCK_EXPECT( GetFlowListWithinCircle );
            MOCK_EXPECT( GetTransporter ).returns( 0 );
            ExpectEffect( entity[ "perceptions/max-agent-perception-distance" ] );
            ExpectEffect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
            ExpectEffect( entity[ "perceptions/peripherical-vision" ], sword::test::MakeModel( "activated", false ) );
            ExpectEffect( entity[ "perceptions/main-perception-direction" ], sword::test::MakeModel( "x", direction.rX_ )
                                                                                                   ( "y", direction.rY_ ) );
            ExpectEffect( entity[ "perceptions/cones" ] );
        }
        typedef std::map< std::string, sword::test::ModelBuilder > T_Nofitications;
        void ExpectNotifications( const T_Nofitications& notifications )
        {
            std::set< std::string > remaining = boost::assign::list_of( "agents" )
                                                                      ( "objects" )
                                                                      ( "population-flows" )
                                                                      ( "population-concentrations" )
                                                                      ( "urban-blocks" );
            BOOST_FOREACH( const T_Nofitications::value_type& notification, notifications )
            {
                ExpectEffect( entity[ "perceptions/notifications" ][ notification.first ], notification.second );
                remaining.erase( notification.first );
            }
            BOOST_FOREACH( const std::string& notification, remaining )
                ExpectEffect( entity[ "perceptions/notifications" ][ notification ] );
        }
        void ExpectNotifications( const std::string& node, const sword::test::ModelBuilder& builder )
        {
            ExpectNotifications( boost::assign::map_list_of( node, builder ) );
        }
        void ExpectNotifications()
        {
            ExpectNotifications( T_Nofitications() );
        }
        const unsigned int target;
    };
}
}

#endif // PERCEPTION_COMMAND_FIXTURE_H
