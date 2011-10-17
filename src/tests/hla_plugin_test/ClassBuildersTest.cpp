// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/ClassBuilders.h"
#include "hla_plugin/HlaObject_ABC.h"
#include <hla/Class.h>  // $$$$ _RC_ SLI 2011-07-26: not mockable
#include "MockFederate.h"
#include "MockObjectRegistration.h" // $$$$ _RC_ SLI 2011-07-26: Don't mock types you don't own
#include "MockAttributeFunctor.h"   // $$$$ _RC_ SLI 2011-07-26: Don't mock types you don't own
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : hlaClass( objectRegistration, true )
        {}
        void Check( ClassBuilder_ABC& builder, const std::string& name )
        {
            MOCK_EXPECT( federate, RegisterClass ).once().with( name, mock::any, mock::any, mock::any );
            builder.Build( federate, hlaClass, true, false );
            mock::sequence s;
            ::hla::MockAttributeFunctor visitor;
            BOOST_FOREACH( const std::string& attribute, attributes )
                MOCK_EXPECT( visitor, Visit ).once().in( s ).with( attribute );
            hlaClass.Apply( visitor );
        }
        MockFederate federate;
        ::hla::MockObjectRegistration< HlaObject_ABC > objectRegistration;
        ::hla::Class< HlaObject_ABC > hlaClass;
        std::vector< std::string > attributes;
    };
}

BOOST_FIXTURE_TEST_CASE( aggegate_entity_builder_registers_attributes, Fixture )
{
    AggregateEntityBuilder builder;
    attributes = boost::assign::list_of( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "AggregateMarking" )
                                       ( "AggregateState" )
                                       ( "Dimensions" )
                                       ( "Spatial" )
                                       ( "Formation" )
                                       ( "NumberOfSilentEntities" )
                                       ( "SilentEntities" )
                                       ( "SilentAggregates" )
                                       ( "SubAggregateIdentifiers" )
                                       ( "EntityIdentifiers" );
    Check( builder, "BaseEntity.AggregateEntity" );
}

BOOST_FIXTURE_TEST_CASE( netn_aggegate_entity_builder_registers_attributes, Fixture )
{
    NetnAggregateEntityBuilder builder;
    attributes = boost::assign::list_of( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "AggregateMarking" )
                                       ( "AggregateState" )
                                       ( "Dimensions" )
                                       ( "Spatial" )
                                       ( "Formation" )
                                       ( "NumberOfSilentEntities" )
                                       ( "SilentEntities" )
                                       ( "SilentAggregates" )
                                       ( "SubAggregateIdentifiers" )
                                       ( "EntityIdentifiers" )
                                       // NETN
                                       ( "Mounted" )
                                       ( "Echelon" )
                                       ( "UniqueID" )
                                       ( "HigherHeadquarters" )
                                       ( "Callsign" )
                                       ( "Status" )
                                       ( "Symbol" );
    Check( builder, "BaseEntity.AggregateEntity.NETN_Aggregate" );
}

BOOST_FIXTURE_TEST_CASE( surface_vessel_builder_registers_attributes, Fixture )
{
    SurfaceVesselBuilder builder;
    attributes = boost::assign::list_of( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.SurfaceVessel" );
}

BOOST_FIXTURE_TEST_CASE( netn_surface_vessel_builder_registers_attributes, Fixture )
{
    NetnSurfaceVesselBuilder builder;
    attributes = boost::assign::list_of( "EntityType" )
                                       ( "EntityIdentifier" )
                                       ( "ForceIdentifier" )
                                       ( "Marking" )
                                       ( "Spatial" )
                                       // NETN
                                       ( "UniqueID" )
                                       ( "Callsign" );
    Check( builder, "BaseEntity.PhysicalEntity.Platform.SurfaceVessel.NETN_SurfaceVessel" );
}
