// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"

#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/XmlAdapter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "script_plugin/ActionScheduler.h"
#include "script_plugin/MiscEvents.h"

#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace plugins::script;

namespace
{
    MOCK_BASE_CLASS( MockPublisher, dispatcher::SimulationPublisher_ABC )
    {
        MOCK_METHOD( Send, 1, void( const sword::DispatcherToSim& ), DispatchToSim );
        MOCK_METHOD( Send, 1, void( const sword::ClientToSim& ),     ClientToSim );
    };

    MOCK_BASE_CLASS( MockResolver, kernel::EntityResolver_ABC )
    {
        MOCK_METHOD( FindAgent, 1 );
        MOCK_METHOD( GetAgent, 1 );
        MOCK_METHOD( FindAutomat, 1 );
        MOCK_METHOD( GetAutomat, 1 );
        MOCK_METHOD( FindInhabitant, 1 );
        MOCK_METHOD( GetInhabitant, 1 );
        MOCK_METHOD( FindKnowledgeGroup, 1 );
        MOCK_METHOD( GetKnowledgeGroup, 1 );
        MOCK_METHOD( FindObject, 1 );
        MOCK_METHOD( GetObject, 1 );
        MOCK_METHOD( FindPopulation, 1 );
        MOCK_METHOD( GetPopulation, 1 );
        MOCK_METHOD( FindFormation, 1 );
        MOCK_METHOD( GetFormation, 1 );
        MOCK_METHOD( FindTeam, 1 );
        MOCK_METHOD( GetTeam, 1 );
        MOCK_METHOD( FindUrbanObject, 1 );
        MOCK_METHOD( GetUrbanObject, 1 );
        MOCK_METHOD( FindPathfind, 1 );
        MOCK_METHOD( FindEntity, 1 );
    };

    struct Fixture
    {
        Fixture()
            : converter( new CoordinateConverter() )
            , adapter  ( *converter, resolver )
        {
            // NOTHING
        }

        MockPublisher                            publisher;
        std::unique_ptr< CoordinateConverter_ABC > converter;
        MockResolver                             resolver;
        const kernel::XmlAdapter                 adapter;
    };
}

BOOST_FIXTURE_TEST_CASE( scheduler_schedules, Fixture )
{
    const std::string input =
    "<actions>"
    "  <action id='teleport' name='Teleport' target='3417' time='2009-11-29T12:03:08' type='magicunit'>"
    "    <parameter name='Location' type='point'>"
    "      <location type='point'>"
    "        <point coordinates='35RPP8147194833'/>"
    "      </location>"
    "    </parameter>"
    "  </action>"
    "  <action id='unit_creation' name='Unit Creation' output='3417' target='158' time='2009-11-29T11:38:28' type='magicunit'>"
    "    <parameter name='UnitType' type='identifier' value='63'/>"
    "    <parameter name='Location' type='point'>"
    "      <location type='point'>"
    "        <point coordinates='35RPQ8447806364'/>"
    "      </location>"
    "    </parameter>"
    "  </action>"
    "</actions>";
    xml::xistringstream xis( input );
    MOCK_EXPECT( resolver.FindAgent ).returns( reinterpret_cast< kernel::Agent_ABC* >( this ) );
    ActionScheduler scheduler( xis, adapter, publisher );
    scheduler.NotifyUpdated( events::SimulationTimeChanged( "2009-11-29T00:00:00" ) );
    MOCK_EXPECT( publisher.ClientToSim ).once();
    scheduler.NotifyUpdated( events::SimulationTimeChanged( "2009-11-29T12:00:00" ) );
    MOCK_EXPECT( publisher.ClientToSim ).once();
    scheduler.NotifyUpdated( events::SimulationTimeChanged( "2009-12-29T12:00:00" ) );
    scheduler.NotifyUpdated( events::SimulationTimeChanged( "2010-12-29T12:00:00" ) );
}

BOOST_FIXTURE_TEST_CASE( scheduler_schedules_multiple_orders_at_same_timestamp, Fixture )
{
    const std::string input =
    "<actions>"
    "  <action id='unit_creation' output='3417' target='158' time='2009-11-29T11:38:28' type='magicunit'/>"
    "  <action id='unit_creation' output='3418' target='159' time='2009-11-29T11:38:28' type='magicunit'/>"
    "</actions>";
    xml::xistringstream xis( input );
    MOCK_EXPECT( resolver.FindAgent ).returns( reinterpret_cast< kernel::Agent_ABC* >( this ) );
    ActionScheduler scheduler( xis, adapter, publisher );
    MOCK_EXPECT( publisher.ClientToSim ).exactly( 2 );
    scheduler.NotifyUpdated( events::SimulationTimeChanged( "2009-11-29T12:00:00" ) );
}
