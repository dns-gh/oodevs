// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "timeline_plugin/RestClient.h"
#include "timeline_plugin/Publisher_ABC.h"
#include "timeline_plugin/ActionLoader.h"
#include "dispatcher/Logger_ABC.h" 
#include "tools/FileWrapper.h"
#include <xeumeuleu/xml.hpp>
#include <iostream>

MOCK_BASE_CLASS( MockPublisher, plugins::timeline::Publisher_ABC )
{
    MOCK_METHOD( PullSituation, 3 )
    MOCK_METHOD( PushReport, 1 )
    MOCK_METHOD( RestartScenario, 0 )
};

namespace dispatcher
{
    MOCK_BASE_CLASS( MockLogger, Logger_ABC )
    {
        MOCK_METHOD( LogInfo, 1 )
        MOCK_METHOD( LogError, 1 )
        MOCK_METHOD( LogWarning, 1 )
    };
}

namespace
{
    std::string Expect( const std::string& data )
    {
        xml::xostringstream xos;
        xos << xml::start( "CommitScenario" ) << xml::attribute( "xmlns", "urn:masa:taranis:scenario:1.0" )
                << xml::attribute( "xmlns:noNamespaceSchemaLocation", "CommitScenario.xsd" )
                << xml::attribute( "id", 1 )
                << xml::start( "CommitTask" ) << xml::attribute( "timestamp", 0 )
                    << xml::start( "task" ) << xml::attribute( "id", -1 ) << xml::attribute( "name", "Execute mission: 'move'" )
                                            << xml::attribute( "actor", 1 ) << xml::attribute( "type", "action" )
                        << xml::xistringstream( data )
                    << xml::end
                << xml::end
                << xml::start( "CommitScheduler" ) << xml::attribute( "timestamp", 0 )
                    << xml::start( "schedule" )
                        << xml::attribute( "id", -1 ) << xml::attribute( "task", -1 )
                        << xml::attribute( "time", "2008-08-11T08:12:20" ) << xml::attribute( "status", "pending" );
        return xos.str();
    }
}

BOOST_AUTO_TEST_CASE( timeline_plugin_action_loader_test )
{
    const tools::Path filename = testOptions.GetDataPath( "timeline_plugin/orders.ord" );
    MockPublisher publisher;
    dispatcher::MockLogger logger;
    MOCK_EXPECT( logger.LogInfo );
    tools::Xifstream xis( filename );
    xis >> xml::start( "actions" ) >> xml::start( "action" );
    xml::xostringstream xos;
    xos << xml::content( "action", xis );
    MOCK_EXPECT( publisher.PushReport ).once().with( Expect( xos.str() ) );
    plugins::timeline::ActionLoader loader( 1, 1, publisher, logger );
    loader.Load( filename );
}
