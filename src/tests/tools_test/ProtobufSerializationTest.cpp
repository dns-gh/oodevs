// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/ProtobufSerialization.h"
#include "protocol/Protocol.h"

const std::string binary = "Hello World!!";
const std::string base64 = "SGVsbG8gV29ybGQhIQ==";

BOOST_AUTO_TEST_CASE( test_base64_proto_encode_decode )
{
    sword::ClientToSim msg;
    sword::UnitOrder* unitOrder = msg.mutable_message()->mutable_unit_order();
    unitOrder->mutable_tasker()->set_id( 42 );
    unitOrder->mutable_type()->set_id( 404 );
    unitOrder->set_label( "test" );
    sword::MissionParameters* parameters = unitOrder->mutable_parameters();
    parameters->add_elem()->set_null_value( true );
    parameters->add_elem()->add_value()->set_booleanvalue( true );
    parameters->add_elem()->add_value()->set_intvalue( 42 );
    parameters->add_elem()->add_value()->mutable_heading()->set_heading( 42 );
    parameters->add_elem()->add_value()->set_booleanvalue( true );
    parameters->add_elem()->add_value()->set_enumeration( 0 );
    sword::Location* location = parameters->add_elem()->add_value()->mutable_point()->mutable_location();
    location->set_type( sword::Location_Geometry_point );
    sword::CoordLatLong* latLong = location->mutable_coordinates()->add_elem();
    latLong->set_latitude( 30.6134 );
    latLong->set_longitude( 29.1151 );

    const sword::ClientToSim msg2 = tools::BinaryToProto< sword::ClientToSim >( tools::ProtoToBinary( msg ) );

    BOOST_CHECK_EQUAL( msg.DebugString(), msg2.DebugString() );
}
