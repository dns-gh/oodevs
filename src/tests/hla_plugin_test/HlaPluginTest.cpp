// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/FederateFacade.h"
#include "MockRtiAmbassadorFactory.h"
#include "MockModel.h"         // $$$$ _RC_ SLI 2011-01-11: Don't mock type you don't own
#include "MockRtiAmbassador.h" // $$$$ _RC_ SLI 2011-01-11: Don't mock type you don't own
#include <hla/HLA_lib.h>
#include <hla/SimpleTime.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace hla
{
    std::ostream& operator<<( std::ostream& os, const hla::ClassIdentifier& identifier )
    {
        return os << identifier.ToString();
    }
}

BOOST_AUTO_TEST_CASE( hla_plugin_initialization_declares_publications )
{
    xml::xistringstream xis( "<root name='name' federation='federation'/>" );
    xis >> xml::start( "root" );
    dispatcher::MockModel model;
    MockRtiAmbassadorFactory factory;
    hla::MockRtiAmbassador* rtiAmbassador( new hla::MockRtiAmbassador() );
    hla::FederateAmbassador_ABC* federateAmbassador = 0;
    hla::SimpleTime target;
    mock::sequence s;
    MOCK_EXPECT( model, RegisterFactory ).once().in( s );
    MOCK_EXPECT( factory, CreateAmbassador ).once().in( s ).with( mock::any, mock::any, hla::RtiAmbassador_ABC::TimeStampOrder, "localhost", "8989" ).returns( std::auto_ptr< hla::RtiAmbassador_ABC >( rtiAmbassador ) );
    MOCK_EXPECT( rtiAmbassador, Join ).once().in( s ).with( "name", "federation", mock::retrieve( federateAmbassador ) ).returns( true );
    MOCK_EXPECT( rtiAmbassador, Tick ).calls( boost::bind( &hla::FederateAmbassador_ABC::TimeAdvanceGranted, boost::ref( federateAmbassador ), boost::ref( target ) ) );;
    MOCK_EXPECT( rtiAmbassador, EnableTimeRegulation ).once().in( s ).calls( boost::bind( &hla::FederateAmbassador_ABC::TimeRegulationEnabled, boost::ref( federateAmbassador ), boost::ref( target ) ) );
    MOCK_EXPECT( rtiAmbassador, PublishClass ).once().in( s ).with( "BaseEntity.AggregateEntity", mock::any );
    FederateFacade facade( xis, model, factory, 0u );
    MOCK_EXPECT( rtiAmbassador, Resign ).once().in( s );
    MOCK_EXPECT( model, UnregisterFactory ).once().in( s );
}
